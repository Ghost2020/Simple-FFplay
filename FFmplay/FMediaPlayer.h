#pragma once

#include <string>

#include "FDecoder.h"

extern "C"
{
#include "config.h"
#include <inttypes.h>
#include <signal.h>
#include <stdint.h>

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavcodec/avfft.h"
#include "libavdevice/avdevice.h"
#include "libswresample/swresample.h"

#if CONFIG_AVFILTER
# include "libavfilter/avfilter.h"
# include "libavfilter/buffersink.h"
# include "libavfilter/buffersrc.h"
#endif

#include <SDL.h>

#include "cmdutils.h"
}

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
/* TODO: We assume that a decoded and resampled frame fits into this buffer */
constexpr uint32_t SAMPLE_ARRAY_SIZE = (8 * 65536);

struct AudioParams
{
    int freq;
    int channels;
    int64_t channel_layout;
    AVSampleFormat fmt;
    int frame_size;
    int bytes_per_sec;
};

class SClock
{
    friend class FMediaPlayer;
public:
    void Init(int* queue_serial);

    double Get();

    void SetAt(double pts, int serial, double time);

    void Set(double pts, int serial);

    void SetSpeed(double speed);

private:
    double pts;           /* clock base */
    double pts_drift;     /* clock base minus time at which we updated the clock */
    double last_updated;
    double speed = 1.0;
    int serial;           /* clock is based on a packet with this serial */
    bool paused = false;
    int* queue_serial = nullptr;    /* pointer to the current packet queue serial, used for obsolete clock detection */
};


class FMediaPlayer
{
public:
    enum struct EShowMode : uint8_t
    {
        SHOW_MODE_NONE = 0, SHOW_MODE_VIDEO, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
    };

    enum struct ESyncType : uint8_t
    {
        AV_SYNC_AUDIO_MASTER, /* default choice */
        AV_SYNC_VIDEO_MASTER,
        AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
    };

public:
    FMediaPlayer();
    ~FMediaPlayer();

    FMediaPlayer(const FMediaPlayer&) = delete;
    FMediaPlayer& operator=(const FMediaPlayer&) = delete;

public:
    static bool InitContext();
  
    static void UnInitContext();
  
    bool InitRender();

    void UninitRender();

    void set_default_window_size(int width, int height, AVRational sar);

    inline void fill_rectangle(int x, int y, int w, int h);

    int realloc_texture(SDL_Texture** texture, Uint32 new_format, int new_width, int new_height, SDL_BlendMode blendmode, int init_texture);

    void calculate_display_rect(SDL_Rect* rect,
        int scr_xleft, int scr_ytop, int scr_width, int scr_height,
        int pic_width, int pic_height, AVRational pic_sar);

    void get_sdl_pix_fmt_and_blendmode(int format, Uint32* sdl_pix_fmt, SDL_BlendMode* sdl_blendmode);

    int upload_texture(SDL_Texture** tex, AVFrame* frame, struct SwsContext** img_convert_ctx);

    void set_sdl_yuv_conversion_mode(AVFrame* frame);

    void video_image_display();

    static inline int compute_mod(int a, int b);

    void video_audio_display();

    void stream_component_close(int stream_index);

    void stream_close();

    void do_exit();

    static void sigterm_handler(int sig);

    int video_open();

    /* display the current picture, if any */
    void video_display();

    static void sync_clock_to_slave(SClock* c, SClock* slave);

    ESyncType get_master_sync_type();

    /* get the current master clock value */
    double get_master_clock();

    void check_external_clock_speed();

    /* seek in the stream */
    void stream_seek(int64_t pos, int64_t rel, int seek_by_bytes);

    /* pause or resume the video */
    void stream_toggle_pause();

    void toggle_pause();

    void toggle_mute();

    void update_volume(int sign, double step);

    void step_to_next_frame();

    double compute_target_delay(double delay);

    double vp_duration(SFrame* vp, SFrame* nextvp);

    void update_video_pts(double pts, int64_t pos, int serial);

    /* called to display each frame */
    void video_refresh(double& remaining_time);
    
    int queue_picture(AVFrame* src_frame, double pts, double duration, int64_t pos, int serial);

    int get_video_frame(AVFrame* frame);

#if CONFIG_AVFILTER
    static int configure_filtergraph(AVFilterGraph* graph, const char* filtergraph,
        AVFilterContext* source_ctx, AVFilterContext* sink_ctx);

    static int configure_video_filters(AVFilterGraph* graph, VideoState* is, const char* vfilters, AVFrame* frame);

    static int configure_audio_filters(VideoState* is, const char* afilters, int force_output_format);
#endif  /* CONFIG_AVFILTER */

    int audio_thread();

    int video_thread();

    int subtitle_thread();

    /* this thread gets the stream from the disk or the network */
    int read_thread();

    int audio_open(int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, struct AudioParams* audio_hw_params);

    /* prepare a new audio buffer */
    static void sdl_audio_callback(void* pUserData, Uint8* stream, int len);

    static int decode_interrupt_cb(void* pUserData);

    /* copy samples for viewing in editor window */
    void update_sample_display(short* samples, int samples_size);

    /* return the wanted number of samples to get better sync if sync_type is video
     * or external master clock */
    int synchronize_audio(int nb_samples);

    /**
     * Decode one audio frame and return its uncompressed size.
     *
     * The processed audio frame is decoded, converted if required, and
     * stored in is->audio_buf, with size in bytes given by the return
     * value.
     */
    int audio_decode_frame();

    /* open a given stream. Return 0 if OK */
    int stream_component_open(int stream_index);

    bool stream_has_enough_packets(AVStream* st, int stream_id, PacketQueue* queue);

    /*
    * \@brief 播放的媒体是否是实时的，需要判断根据协议来判断
    */
    bool is_realtime();

    bool stream_open(const char* filename);

    void stream_cycle_channel(int codec_type);

    void toggle_full_screen();

    void toggle_audio_display();

    void refresh_loop_wait_event(SDL_Event* event);

    void seek_chapter(int incr);

    /* handle an event sent by the GUI */
    void event_loop();

public:
    static EShowMode eShow_mode;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_RendererInfo renderer_info = { 0 };
    SDL_AudioDeviceID audio_dev;

    AVPacket flush_pkt;
    /* 用以获取线程执行的结果，以及线程同步 */
    std::future<int> future;
    AVInputFormat* iformat = nullptr;
    bool abort_request = false;
    int force_refresh;
    bool paused = false;
    int last_paused;
    int queue_attachments_req;
    int seek_req;
    int seek_flags;
    int loop = 1;
    bool framedrop = false;
    /* 无限缓冲区 */
    bool infinite_buffer = false;
    uint8_t startup_volume = 100;
    int64_t seek_pos;
    int64_t seek_rel;
    int read_pause_return;
    AVFormatContext* ic = nullptr;
    bool realtime = false;

    /* 是否自动退出 */
    bool autoexit = false;

    int alwaysontop = false;

    const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = { 0 };

    unsigned sws_flags = SWS_BICUBIC;

    int decoder_reorder_pts = -1;

    int lowres = 0;

#if CONFIG_AVFILTER
    static const char** vfilters_list = nullptr;
    static int nb_vfilters = 0;
    static char* afilters = nullptr;
#endif
    int autorotate = 1;
    /* number of filter threads per graph */
    int filter_nbthreads = 0;

    /* current context */
    int is_full_screen;
    int64_t audio_callback_time;

    int64_t cursor_last_shown;
    int cursor_hidden = 0;

    int64_t start_time = AV_NOPTS_VALUE;
    int64_t duration = AV_NOPTS_VALUE;

    uint16_t default_width = 640;
    uint16_t default_height = 480;
    uint16_t screen_width = 0;
    uint16_t screen_height = 0;
    uint16_t screen_left = SDL_WINDOWPOS_CENTERED;
    uint16_t screen_top = SDL_WINDOWPOS_CENTERED;

    int seek_by_bytes = -1;
    float seek_interval = 10;

    double rdftspeed = 0.02;

    SClock audclk;
    SClock vidclk;
    SClock extclk;

    FrameQueue pictq;
    FrameQueue subpq;
    FrameQueue sampq;

    FDecoder auddec;
    FDecoder viddec;
    FDecoder subdec;

    int audio_stream = -1;

    ESyncType av_sync_type;

    double audio_clock;
    int audio_clock_serial;
    double audio_diff_cum; /* used for AV difference average computation */
    double audio_diff_avg_coef;
    double audio_diff_threshold;
    int audio_diff_avg_count;
    AVStream* audio_st = nullptr;
    PacketQueue audioq;
    int audio_hw_buf_size;
    uint8_t* audio_buf = nullptr;
    uint8_t* audio_buf1 = nullptr;
    unsigned int audio_buf_size; /* in bytes */
    unsigned int audio_buf1_size;
    int audio_buf_index; /* in bytes */
    int audio_write_buf_size;
    int audio_volume;
    int muted;
    struct AudioParams audio_src;
#if CONFIG_AVFILTER
    struct AudioParams audio_filter_src;
#endif
    struct AudioParams audio_tgt;
    struct SwrContext* swr_ctx;
    int frame_drops_early;
    int frame_drops_late;

    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index;
    int last_i_start;
    RDFTContext* rdft = nullptr;
    int rdft_bits;
    FFTSample* rdft_data = nullptr;
    int xpos;
    double last_vis_time;
    SDL_Texture* vis_texture = nullptr;
    SDL_Texture* sub_texture = nullptr;
    SDL_Texture* vid_texture = nullptr;

    int subtitle_stream = -1;
    AVStream* subtitle_st = nullptr;
    PacketQueue subtitleq;

    double frame_timer;
    double frame_last_returned_time;
    double frame_last_filter_delay;
    int video_stream = -1;
    AVStream* video_st = nullptr;
    PacketQueue videoq;
    double max_frame_duration;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
    struct SwsContext* img_convert_ctx = nullptr;
    struct SwsContext* sub_convert_ctx = nullptr;
    int eof;

    char* sURL = nullptr;
    int width, height, xleft, ytop;
    int step;

#if CONFIG_AVFILTER
    int vfilter_idx;
    AVFilterContext* in_video_filter = nullptr;   // the first filter in the video chain
    AVFilterContext* out_video_filter = nullptr;  // the last filter in the video chain
    AVFilterContext* in_audio_filter = nullptr;   // the first filter in the audio chain
    AVFilterContext* out_audio_filter = nullptr;  // the last filter in the audio chain
    AVFilterGraph* agraph = nullptr;              // audio filter graph
#endif

    int last_video_stream = -1, last_audio_stream = -1, last_subtitle_stream = -1;

    std::shared_ptr<std::condition_variable> continue_read_thread = nullptr;
};