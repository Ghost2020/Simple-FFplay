#include "FDecoder.h"

FDecoder::FDecoder() {};

void FDecoder::Init(AVCodecContext* avctx, PacketQueue* queue, std::shared_ptr<std::condition_variable> empty_queue_cond)
{
	this->avctx = avctx;
	this->queue = queue;
	this->empty_queue_cond = empty_queue_cond;
	this->start_pts = AV_NOPTS_VALUE;
	this->pkt_serial = -1;
}

int FDecoder::Decode(AVFrame* frame, AVSubtitle* sub)
{
	int ret = AVERROR(EAGAIN);

	for (;;)
	{
		AVPacket pkt;

		if (this->queue->serial == this->pkt_serial)
		{
			do
			{
				if (this->queue->abort_request)
					return -1;

				switch (this->avctx->codec_type)
				{
				case AVMEDIA_TYPE_VIDEO:
					ret = avcodec_receive_frame(this->avctx, frame);
					if (ret >= 0)
					{
						/*if (decoder_reorder_pts == -1)*/
						frame->pts = frame->best_effort_timestamp;

						/* else if (!decoder_reorder_pts)
							 frame->pts = frame->pkt_dts;*/
					}
					break;
				case AVMEDIA_TYPE_AUDIO:
					ret = avcodec_receive_frame(this->avctx, frame);
					if (ret >= 0) {
						AVRational tb = AVRational{ 1, frame->sample_rate };
						if (frame->pts != AV_NOPTS_VALUE)
							frame->pts = av_rescale_q(frame->pts, this->avctx->pkt_timebase, tb);
						else if (this->next_pts != AV_NOPTS_VALUE)
							frame->pts = av_rescale_q(this->next_pts, this->next_pts_tb, tb);
						if (frame->pts != AV_NOPTS_VALUE) {
							this->next_pts = frame->pts + frame->nb_samples;
							this->next_pts_tb = tb;
						}
					}
					break;
				}
				if (ret == AVERROR_EOF)
				{
					this->finished = this->pkt_serial;
					avcodec_flush_buffers(this->avctx);
					return 0;
				}
				if (ret >= 0)
					return 1;
			} while (ret != AVERROR(EAGAIN));
		}

		do
		{
			if (this->queue->nb_packets == 0)
				this->empty_queue_cond->notify_one();
			if (this->packet_pending) {
				av_packet_move_ref(&pkt, &this->pkt);
				this->packet_pending = false;
			}
			else {
				if (!this->queue->Get(&pkt, 1, &this->pkt_serial))
					return -1;
			}
		} while (this->queue->serial != this->pkt_serial);

		if (pkt.data == this->queue->_flush_pkt.data) {
			avcodec_flush_buffers(this->avctx);
			this->finished = 0;
			this->next_pts = this->start_pts;
			this->next_pts_tb = this->start_pts_tb;
		}
		else
		{
			if (this->avctx->codec_type == AVMEDIA_TYPE_SUBTITLE) {
				int got_frame = 0;
				ret = avcodec_decode_subtitle2(this->avctx, sub, &got_frame, &pkt);
				if (ret < 0)
					ret = AVERROR(EAGAIN);
				else {
					if (got_frame && !pkt.data) {
						this->packet_pending = true;
						av_packet_move_ref(&this->pkt, &pkt);
					}
					ret = got_frame ? 0 : (pkt.data ? AVERROR(EAGAIN) : AVERROR_EOF);
				}
			}
			else {
				if (avcodec_send_packet(this->avctx, &pkt) == AVERROR(EAGAIN)) {
					av_log(this->avctx, AV_LOG_ERROR, "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
					this->packet_pending = true;
					av_packet_move_ref(&this->pkt, &pkt);
				}
			}
			av_packet_unref(&pkt);
		}
	}
}

int FDecoder::Start(std::function<int(void)>& func)
{
	this->queue->Start();

	this->future = std::async(std::launch::async, std::move(func));
	if (!this->future.valid())
	{
		av_log(nullptr, AV_LOG_ERROR, "CreateThread Failured!\n");
		return AVERROR(ENOMEM);
	}
	return 0;
}

void FDecoder::Destroy()
{
	av_packet_unref(&this->pkt);
	avcodec_free_context(&this->avctx);
}

void FDecoder::Abort(FrameQueue* fq)
{
	this->queue->Abort();
	fq->Signal();

	if (future.valid())
	{
		if (future.get() != 0)
			av_log(nullptr, AV_LOG_WARNING, "Thread exit exception\n");
	}

	this->queue->Flush();
}