#pragma once

#include <memory>
#include <mutex>
#include <future>
#include <condition_variable>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/time.h"
}

constexpr uint32_t VIDEO_PICTURE_QUEUE_SIZE = 3;
constexpr uint32_t SUBPICTURE_QUEUE_SIZE = 16;
constexpr uint32_t SAMPLE_QUEUE_SIZE = 9;
constexpr uint32_t FRAME_QUEUE_SIZE = FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE));

struct MyAVPacketList {
	AVPacket pkt;
	int serial = -1;
	struct MyAVPacketList* next = nullptr;
};

class PacketQueue
{
	friend class FrameQueue;
	friend class FMediaPlayer;
	friend class FDecoder;
public:
	/* packet queue handling */
	void Init(AVPacket& packet)
	{
		_flush_pkt = packet;
		abort_request = true;
	}

	int PutPrivate(AVPacket* pkt)
	{
		MyAVPacketList* pkt1 = nullptr;

		if (abort_request)
			return -1;

		pkt1 = static_cast<MyAVPacketList*>(av_malloc(sizeof(MyAVPacketList)));
		if (!pkt1)
			return -1;
		pkt1->pkt = *pkt;
		pkt1->next = nullptr;
		if (pkt == &_flush_pkt)
			serial++;
		pkt1->serial = serial;

		if (!last_pkt)
			first_pkt = pkt1;
		else
			last_pkt->next = pkt1;
		last_pkt = pkt1;
		nb_packets++;
		size += pkt1->pkt.size + sizeof(*pkt1);
		duration += pkt1->pkt.duration;
		/* XXX: should duplicate packet data in DV case */
		cond.notify_one();
		return 0;
	}

	int Put(AVPacket* pkt)
	{
		int ret;

		{
			std::lock_guard<std::mutex> lock(mutex);
			ret = PutPrivate(pkt);
		}

		if (pkt != &_flush_pkt && ret < 0)
			av_packet_unref(pkt);

		return ret;
	}

	int PutNullPacket(int stream_index)
	{
		AVPacket pkt1, * pkt = &pkt1;
		av_init_packet(pkt);
		pkt->data = nullptr;
		pkt->size = 0;
		pkt->stream_index = stream_index;
		return Put(pkt);
	}

	void Flush()
	{
		MyAVPacketList* pkt = nullptr, * pkt1 = nullptr;

		std::lock_guard<std::mutex> lock(mutex);
		for (pkt = first_pkt; pkt; pkt = pkt1)
		{
			pkt1 = pkt->next;
			av_packet_unref(&pkt->pkt);
			av_freep(&pkt);
		}
		first_pkt = last_pkt = nullptr;
		nb_packets = size = duration = 0;
	}

	void Destroy()
	{
		Flush();
	}

	void Abort()
	{
		std::lock_guard<std::mutex> lock(mutex);
		this->abort_request = true;

		cond.notify_one();
	}

	void Start()
	{
		std::lock_guard<std::mutex> lock(mutex);
		abort_request = false;
		PutPrivate(&_flush_pkt);
	}

	/* 
	* \@breif 
	* \@return return false if aborted, 0 if no packet and > 0 if packet
	*/
	bool Get(AVPacket* pkt, int block, int* serial)
	{
		MyAVPacketList* pkt1 = nullptr;
		bool ret = false;

		std::unique_lock<std::mutex> lock(mutex);

		for (;;) 
		{
			if (abort_request) 
			{
				ret = false;
				break;
			}

			pkt1 = first_pkt;
			if (pkt1) {
				first_pkt = pkt1->next;
				if (!first_pkt)
					last_pkt = nullptr;
				nb_packets--;
				size -= pkt1->pkt.size + sizeof(*pkt1);
				duration -= pkt1->pkt.duration;
				*pkt = pkt1->pkt;
				if (serial)
					*serial = pkt1->serial;
				av_free(pkt1);
				ret = true;
				break;
			}
			else if (!block) {
				ret = true;
				break;
			}
			else {
				cond.wait(lock);
			}
		}

		return ret;
	}

private:
	MyAVPacketList* first_pkt = nullptr, * last_pkt = nullptr;
	AVPacket _flush_pkt;
	int nb_packets = 0;
	int size = 0;
	int64_t duration = 0;
	bool abort_request = false;
	int serial;
	std::mutex mutex;
	std::condition_variable cond;
};

/* Common struct for handling all types of decoded data and allocated render buffers. */
struct SFrame
{
	/* 原始图像数据 */
	AVFrame* frame = nullptr;
	/* 字幕相关数据 */
	AVSubtitle sub{};
	int serial;
	double pts;           /* presentation timestamp for the frame */
	double duration;      /* estimated duration of the frame */
	int64_t pos;          /* byte position of the frame in the input file */
	int width;
	int height;
	int format;
	AVRational sar{};
	bool uploaded = false;
	int flip_v;
};

class FrameQueue
{
public:
	FrameQueue() {};

	FrameQueue(const FrameQueue&) = delete;
	FrameQueue& operator=(const FrameQueue&) = delete;
public:

	bool Init(PacketQueue* pktq, int max_size, int keep_last)
	{
		this->pktq = pktq;
		this->max_size = FFMIN(max_size, FRAME_QUEUE_SIZE);
		this->keep_last = !!keep_last;
		for (int i = 0; i < max_size; i++)
			if (!(queue[i].frame = av_frame_alloc()))
				return false;
		return true;
	}

	void Destory()
	{
		for (int i = 0; i < max_size; i++)
		{
			SFrame* vp = &queue[i];
			UnrefItem(vp);
			av_frame_free(&vp->frame);
		}
	}

	void Signal()
	{
		std::lock_guard<std::mutex> lock(mutex);
		cond.notify_one();
	}

	SFrame* Peek()
	{
		return &queue[(rindex + rindex_shown) % max_size];
	}

	SFrame* PeekNext()
	{
		return &queue[(rindex + rindex_shown + 1) % max_size];
	}

	SFrame* PeekLast()
	{
		return &queue[rindex];
	}

	SFrame* PeekWritable()
	{
		/* wait until we have space to put a new frame */
		{
			std::unique_lock<std::mutex> lock(mutex);
			while (size >= max_size && !pktq->abort_request)
				cond.wait(lock);
		}

		if (pktq->abort_request)
			return nullptr;

		return &queue[windex];
	}

	SFrame* PeekReadable()
	{
		/* wait until we have a readable a new frame */
		{
			std::unique_lock<std::mutex> lock(mutex);
			while (size - rindex_shown <= 0 && !pktq->abort_request)
				cond.wait(lock);
		}

		if (pktq->abort_request)
			return nullptr;

		return &queue[(rindex + rindex_shown) % max_size];
	}

	void Push()
	{
		if (++windex == max_size)
			windex = 0;

		std::lock_guard<std::mutex> lock(mutex);
		size++;
		cond.notify_one();
	}

	void Next()
	{
		if (keep_last && !rindex_shown)
		{
			rindex_shown = 1;
			return;
		}
		UnrefItem(&queue[rindex]);
		if (++rindex == max_size)
			rindex = 0;

		std::lock_guard<std::mutex> lock(mutex);
		size--;
		cond.notify_one();
	}

	/* return the number of undisplayed frames in the queue */
	int NbRemaining()
	{
		return size - rindex_shown;
	}

	/* return last shown position */
	int64_t LastPos()
	{
		SFrame* fp = &queue[rindex];
		if (rindex_shown && fp->serial == pktq->serial)
			return fp->pos;
		else
			return -1;
	}

	void UnrefItem(SFrame* vp)
	{
		av_frame_unref(vp->frame);
		avsubtitle_free(&vp->sub);
	}

	SFrame queue[FRAME_QUEUE_SIZE];
	int rindex = 0;
	int windex = 0;
	int size = 0;
	int max_size = 0;
	int keep_last = 0;
	int rindex_shown = 0;
	std::mutex mutex;
	std::condition_variable cond;
	PacketQueue* pktq = nullptr;
};

/*
* \@brief 解码相关操作
*/
class FDecoder
{
	friend class FMediaPlayer;
public:
	explicit FDecoder();
	~FDecoder();

	FDecoder(const FDecoder&) = delete;
	FDecoder& operator=(const FDecoder&) = delete;

public:
	/* 是否std::shared_ptr 要传引用 */
	void Init(AVCodecContext* avctx, PacketQueue* queue, std::shared_ptr<std::condition_variable> empty_queue_cond);

	int Decode(AVFrame* frame, AVSubtitle* sub);

	int Start(std::function<int(void)>& func);

	void Destroy();

	void Abort(FrameQueue* fq);

private:
	AVPacket pkt;
	PacketQueue* pQueue = nullptr;
	AVCodecContext* pAvCtx = nullptr;
	int pkt_serial;
	int finished;
	bool packet_pending = false;
	std::shared_ptr<std::condition_variable> empty_queue_cond = nullptr;
	int64_t start_pts;
	AVRational start_pts_tb;
	int64_t next_pts;
	AVRational next_pts_tb;
	std::future<int> future;
};

