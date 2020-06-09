/**
* \@brief Author			Ghost Chen
* \@brief Email				cxx2020@outlook.com
* \@brief Date				2020/05/20
* \@brief File				FMediaPlayer.h
* \@brief Desc:				Encapsulated the ffplay
*/
#pragma once

#include <string>

#include "FDecoder.h"

extern "C"
{
#include <signal.h>
#include <stdint.h>

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavcodec/avfft.h"
#include "libavdevice/avdevice.h"
#include "libswresample/swresample.h"
#include "libavutil/display.h"

#if CONFIG_AVFILTER
# include "libavfilter/avfilter.h"
# include "libavfilter/buffersink.h"
# include "libavfilter/buffersrc.h"
#endif

#include <SDL2/SDL.h>
}

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
/* TODO: We assume that a decoded and resampled frame fits into this buffer */
constexpr uint32_t SAMPLE_ARRAY_SIZE = (8 * 65536);

/*
* \@brief 用于媒体间同步用
*/
class SClock
{
    friend class FMediaPlayer;
public:
    void Init(int* queue_serial);

    double Get();

    void SetAt(double pts, int serial, double time);

    void Set(double pts, int serial);

    /*
    * \@brief 设置时钟速度
    * \@param speed[in]:
    */
    void SetSpeed(double speed);

private:
    double pts = 0;                 /* clock base */
    double pts_drift = 0;           /* clock base minus time at which we updated the clock */
    double last_updated = 0;
    double speed = 1.0;
    int serial = -1;                /* clock is based on a packet with this serial */
    bool paused = false;
    int* queue_serial = nullptr;    /* pointer to the current packet queue serial, used for obsolete clock detection */
};


/* Step size for volume control in dB */
constexpr double SDL_VOLUME_STEP = (0.75);
/* polls for possible required screen refresh at least this often, should be less than 1/fps */
constexpr double REFRESH_RATE = 0.01;

constexpr uint32_t CURSOR_HIDE_DELAY = 1000000;

/*
* \@brief 媒体播放接口
*/
class FMediaPlayer
{
    friend class MainWindow;
    friend class QMediaPlayer;
public:
    /* 显示方式 */
    enum struct EShowMode : uint8_t
    {
        SHOW_MODE_NONE = 0, 
        SHOW_MODE_VIDEO, /* 显示音视频 */
        SHOW_MODE_WAVES, /* 显示声纹波形 */
        SHOW_MODE_RDFT,  /* @TODO */
        SHOW_MODE_NB     /* 效果等同于SHOW_MODE_RDFT */
    };

    /* 音画同步方式 */
    enum struct ESyncType : uint8_t
    {
        AV_SYNC_AUDIO_MASTER,   /* 音频为基准同步 */
        AV_SYNC_VIDEO_MASTER,   /* 视频为基准同步 */
        AV_SYNC_EXTERNAL_CLOCK, /* 外部时钟同步 */
    };

private:
    struct AudioParams
    {
        int freq = -1;
        int channels = -1;
        /* @TODO */
        int64_t channel_layout = -1;
        AVSampleFormat fmt;
        int frame_size = 0;
        int bytes_per_sec = 0;
    };

public:
    /*
    * \@brief construction
    * \@param windowID[in]:window ID
    */
    explicit FMediaPlayer(void* constwindowID = nullptr);
    ~FMediaPlayer();

    FMediaPlayer(const FMediaPlayer&) = delete;
    FMediaPlayer& operator=(const FMediaPlayer&) = delete;

private:
    /* 被实例化的数量 用于自动管理核心资源*/
    static uint8_t g_nInstance;

    /*  */

    /*
    * \@brief 初始化FFmpeg、SDL
    */
    static bool initContext();

    /*
    * \@brief 反初始化环境
    */
    static void unInitContext();

    /*
    * \@brief 初始化渲染相关
    */
    bool initRender();

    /*
    * \@brief 反初始化渲染相关
    */
    void uninitRender();
public:

    /*
    * \@brief 打开媒体流
    * \@param sURL[in]:媒体流的存放位置，或者是直播的地址，也可是本地的摄像头
    * \@param iformat:[in] 指定输入的格式，默认可以设置为nullptr
    * \@return true::打开成功 false::失败
    */
    bool OnStreamOpen(const std::string& sURL, AVInputFormat* iformat = nullptr);

    /*
    * \@brief 关闭媒体流
    */
    void StreamClose();

    /*
    * \@brief handle an event sent by the GUI
    * \@return true: normal false: exit
    */
    bool OnTick();

    /*
   * \@brief
   */
    void OnToggleFullScreen();

    /*
    * \@brief
    */
    void OnToggleAudioDisplay();

    /*
    * \@brief
    */
    void OnTogglePause();

    /*
    * \@brief
    */
    void OnToggleMute();

    /*
    * \@brief 逐帧播放
    */
    void OnStepToNextFrame();

    /*
    * \@breif
    * \@param incr[in]
    */
    void OnSeekChapter(int incr);

    /*
    * \@brief @TODO seek in the stream 
    * \@param pos[in]
    * \@param rel[in]
    * \@param seek_by_bytes[in] true:按字节跳转 false:
    */
    void OnStreamSeek(int64_t pos, int64_t rel, bool seek_by_bytes);

    /*
    * \@brief @TODO
    * \@param codec_type[in]
    */
    void OnStreamCycleChannel(AVMediaType codec_type);

    /*
    * \@brief 调节播放的音量
    * \@param sign[in]: 大小方向
    * \@param step[in]: 音量大小
    */
    void OnUpdateVolume(int sign, double step);

    /*
    * \@brief 设置窗口的默认大小
    * \@param width[in] 窗口宽度
    * \@param height[in] 窗口高度
    * \@param sar[in]
    */
    void OnWindowSizeChange(int width, int height, AVRational sar);

    /*
    * \@brief 退出操作
    */
    void OnExit();

    /*
    * \@brief @TODO
    */
    static void sigterm_handler(int sig);

    /*
    * \@brief 刷新交互事件,以获取事件类型
    * \@param event[in, out]
    */
    void refresh_loop_wait_event(SDL_Event& event);

private:
    /*
    * \@brief 填充矩形，用于显示声纹波形
    * \@param nXPos[in]：
    * \@param nYPos[in]：
    * \@param nWidth[in]：矩形宽度
    * \@param nHeight[in]：矩形高度
    */
    inline void fill_rectangle(int nXPos, int nYPos, int nWidth, int nHeight);

    /*
    * \@brief @TODO
    * \@param texture
    * \@param nNewFormat[in]
    * \@param nNewWidth[in]
    * \@param nNewHeight[in]
    * \@param eBlendmode[in]
    * \@param bInitTexture[in]
    * \@retrun 
    */
    bool realloc_texture(SDL_Texture** texture, Uint32 nNewFormat, int nNewWidth, int nNewHeight, SDL_BlendMode eBlendmode, bool bInitTexture);

    /*
    * \@brief 
    * \@param rect[in,out]
    * \@param srcRect[in]
    * \@param pic_width[in]
    * \@param pic_height[in]
    * \@param pic_sar[in]
    */
    void calculate_display_rect(SDL_Rect& rect, const SDL_Rect& srcRect, int pic_width, int pic_height, AVRational pic_sar);

    /*
    * \@brief 获取SDL 渲染的模式
    * \@param nFormat[in]
    * \@param sdl_pix_fmt[in,out]
    * \@param sdl_blendmode[in,out]
    */
    void get_sdl_pix_fmt_and_blendmode(int nFormat, Uint32* sdl_pix_fmt, SDL_BlendMode* sdl_blendmode);

    /*
    * \@brief 设置SDL-YUV转换的模式
    * \@param frame[in]:原始图像数据
    */
    void set_sdl_yuv_conversion_mode(AVFrame* frame);

    /*
    * \@brief 由外部传入的原始图像数据来更新纹理数据
    * \@param tex[in,out]
    * \@param frame[in]:传入的原始视频数据
    * \@param img_convert_ctx[in,out]
    * \@retrun true:successed false:failured
    */
    bool upload_texture(SDL_Texture** tex, AVFrame* frame, struct SwsContext** img_convert_ctx);

    /* 
    * \@breif 显示当前图片（如果有) 
    */
    void video_display();

    /*
    * \@brief 绘制图像和字幕数据
    */
    void video_image_display();

    /*
    * \@brief 绘制音频波形数据
    */
    void video_audio_display();

    /*
    * \@brief 取余运算
    */
    static inline int compute_mod(int a, int b);

    /*
    * \@brief 关闭指定通道的媒体流
    * \@param nStreamIndex[in] 媒体通道索引
    */
    void stream_component_close(int nStreamIndex);

    /*
    * \@brief 查找媒体流的相关信息 @TODO
    * \@param s[in]
    * \@param codec_opts[in]
    * \@return @TODO
    */
    AVDictionary** setup_find_stream_info_opts(AVFormatContext* s, AVDictionary* codec_opts);

    /*
    * \@brief @TODO 同步时钟
    * \@param c[in]
    * \@param slave[in]
    */
    static void sync_clock_to_slave(SClock& c, SClock& slave);

    /*
    * \@brief 获取同步类型
    */
    ESyncType get_master_sync_type();

    /* 
    * \@brief 获取当前主时钟值 
    */
    double get_master_clock();

    /*
    * \brief @TODO
    */
    void check_external_clock_speed();

    /* 
    * \@brief pause or resume the video 
    */
    void stream_toggle_pause();

    /*
    * \@brief 计算延迟有多大
    * \@param delay[in] 上一帧的持续时间
    * \@return 延迟时间
    */
    double compute_target_delay(double delay);

    /*
    * \@breif 计算临近帧的持续时间
    * \@param vp[in] 
    * \@param nextvp[in]
    * \@return 
    */
    double vp_duration(SFrame* vp, SFrame* nextvp);

    /*
    * \@brief 更新视频帧的时间戳
    * \@param pts[in]
    * \@param pos[in]
    * \@param serial[in]
    */
    void update_video_pts(double pts, int64_t pos, int serial);

    /* 
    * \@brief called to display each frame 
    * \@param remaining_time[in,out]
    */
    void video_refresh(double& remaining_time);
    
    /*
    * \@brief @TODO
    * \@param src_frame[in]
    * \@param remaining_time[in]
    * \@param pts[in]
    * \@param duration[in]
    * \@param pos[in]
    * \@param serial[in]
    * \@return
    */
    int queue_picture(AVFrame* src_frame, double pts, double duration, int64_t pos, int serial);

    /*
    * \@brief @TODO
    * \@param frame[in,out]
    * \@return
    */
    int get_video_frame(AVFrame* frame);

#if CONFIG_AVFILTER
    static inline
        int64_t get_valid_channel_layout(int64_t channel_layout, int channels);

    static inline
        int cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
            enum AVSampleFormat fmt2, int64_t channel_count2);

    static double get_rotation(AVStream* st);

    static int configure_filtergraph(AVFilterGraph* graph, const char* filtergraph,
                                     AVFilterContext* source_ctx, AVFilterContext* sink_ctx);

    int configure_video_filters(AVFilterGraph* graph, const char* vfilters, AVFrame* frame);

    int configure_audio_filters(const char* afilters, int force_output_format);
#endif  /* CONFIG_AVFILTER */

    /*
    * \@brief 用于获取音频数据
    * \@return 0::Normal exit 
    */
    int audio_thread();

    /*
    * \@brief 用于获取视频数据
    * \@return 0::Normal exit 
    */
    int video_thread();

    /*
    * \@brief 用于获取字幕数据
    * \@return 0::Normal exit 
    */
    int subtitle_thread();

    /* 
    * \@brief 此线程从磁盘或网络获取流
    * \@return 0::Normal exit 
    */
    int read_thread();

    /*
    * \@brief 初始化显示窗口<渲染相关>
    * \@return true::successed false::failured
    */
    bool video_open();

    /*
    * \@brief 初始化音频设备
    * \@param wanted_channel_layout[in]
    * \@param wanted_nb_channels[in]
    * \@param wanted_sample_rate[in] 采样率
    * \@param audio_hw_params[in,out] 
    * \@return -1:Failured 其他:Audio buffer size in bytes
    */
    int audio_open(int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate, struct AudioParams* audio_hw_params);

    /* 
    * \@brief prepare a new audio buffer
    * \@param pUserData[in] 当前实例指针
    * \@param stream[in] 
    * \@param len[in]
    */
    static void sdl_audio_callback(void* pUserData, Uint8* stream, int len);

    /*
    * \@brief 终止回调函数
    * \@param pUserData[in] 实例指针数据
    */
    static int decode_interrupt_cb(void* pUserData);

    /* 
    * \@brief @TODO copy samples for viewing in editor window 
    * \@param samples[in]
    * \@param samples_size[in]
    */
    void update_sample_display(short* samples, int samples_size);

    /* 
    * \@brief @TODO return the wanted number of samples to get better sync if sync_type is video or external master clock 
    * \@param nb_samples
    * \@return 
    */
    int synchronize_audio(int nb_samples);

    /**
     * Decode one audio frame and return its uncompressed size.
     *
     * The processed audio frame is decoded, converted if required, and
     * stored in is->audio_buf, with size in bytes given by the return
     * value.
     */
    int audio_decode_frame();

    /* 
    * \@brief open a given stream. 
    * \@param stream_index[in]
    * \@return Return 0 if OK 
    */
    int stream_component_open(int stream_index);

    /*
    * \@brief 用于判定指定流队列中是否有足够的packets数据
    * \@param st[in]
    * \@param stream_id[in]
    * \@param queue[in]
    * \@return true::有 false::无
    */
    bool stream_has_enough_packets(AVStream* st, int stream_id, PacketQueue* queue);

    /*
    * \@brief 播放的媒体是否是实时的，需要判断根据协议来判断
    * \@return true::实时流 false::本地媒体流
    */
    bool is_realtime();

public:
    static EShowMode eShow_mode;

private:
#pragma region Rendering
    void* m_pWindowID = nullptr;
    SDL_Window* pWindow = nullptr;
    SDL_Renderer* pRenderer = nullptr;
    SDL_RendererInfo renderer_info;
    SDL_AudioDeviceID audio_dev = 0;

    SDL_Texture* vis_texture = nullptr;
    /* 字幕纹理 */
    SDL_Texture* sub_texture = nullptr;
    /* 图像纹理 */
    SDL_Texture* vid_texture = nullptr;

    uint16_t default_width = 640;
    uint16_t default_height = 480;
    uint16_t screen_width = 0;
    uint16_t screen_height = 0;
    uint32_t screen_left = SDL_WINDOWPOS_CENTERED;
    uint32_t screen_top = SDL_WINDOWPOS_CENTERED;
#pragma endregion Rendering

    /* @TODO */
    AVPacket flush_pkt;
    /* 用以获取线程执行的结果，以及线程同步 */
    std::future<int> future;
    /*  */
    AVInputFormat* pInputformat = nullptr;
    bool abort_request = false;
    bool force_refresh = false;
    bool paused = false;
    bool last_paused = false;
    bool queue_attachments_req = false;
    bool seek_req = false;
    int seek_flags = 0;
    int loop = 1;
    bool framedrop = false;
    /* 无限缓冲区 */
    bool infinite_buffer = false;
    int64_t seek_pos = 0;
    int64_t seek_rel = 0;
    int read_pause_return = 0;
    /*  */
    AVFormatContext* pFormatCtx = nullptr;

    /* 是否自动退出 */
    bool autoexit = false;

    const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = { 0 };

    unsigned sws_flags = SWS_BICUBIC;

    int decoder_reorder_pts = -1;

    int lowres = 0;

#if CONFIG_AVFILTER
    const char** vfilters_list = nullptr;
    int nb_vfilters = 0;
    char* afilters = nullptr;
#endif
    int autorotate = 1;
    /* number of filter threads per graph */
    int filter_nbthreads = 0;

    /* current context */
    bool is_full_screen = false;
    int64_t audio_callback_time = 0;

    /* 鼠标上次显示的时间 */
    int64_t cursor_last_shown = 0;
    bool cursor_hidden = false;

    int64_t start_time = AV_NOPTS_VALUE;
    int64_t duration = AV_NOPTS_VALUE;

    bool seek_by_bytes = false;
    float seek_interval = 10;

    double rdftspeed = 0.02;

    SClock audclk;  /* 音频时钟 */
    SClock vidclk;  /* 视频时钟 */
    SClock extclk;  /* 外部时钟 */

    FrameQueue pictq;
    FrameQueue subpq;
    FrameQueue sampq;

    FDecoder auddec;
    FDecoder viddec;
    FDecoder subdec;

    int audio_stream = -1;

    /* 音画同步类型 */
    ESyncType av_sync_type;

    double audio_clock = 0;
    int audio_clock_serial = -1;
    double audio_diff_cum = 0; /* used for AV difference average computation */
    double audio_diff_avg_coef = 0;
    double audio_diff_threshold = 0;
    int audio_diff_avg_count = 0;
    AVStream* pAudioStream = nullptr;
    PacketQueue audioq;
    int audio_hw_buf_size = 0;
    uint8_t* audio_buf = nullptr;
    uint8_t* audio_buf1 = nullptr;
    unsigned int audio_buf_size = 0; /* in bytes */
    unsigned int audio_buf1_size = 0;
    int audio_buf_index = 0; /* in bytes */
    int audio_write_buf_size = 0;
    /* 音量<0~100> */
    uint8_t audio_volume = 100;
    bool muted = false;
    struct AudioParams audio_src;
#if CONFIG_AVFILTER
    struct AudioParams audio_filter_src;
#endif
    struct AudioParams audio_tgt;
    struct SwrContext* swr_ctx = nullptr;
    int frame_drops_early;
    int frame_drops_late;

    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index = 0;
    int last_i_start = 0;
    RDFTContext* rdft = nullptr;
    int rdft_bits = 0;
    FFTSample* rdft_data = nullptr;
    int xpos = 0;
    double last_vis_time = 0;

    int subtitle_stream = -1;
    AVStream* pSubtitleStream = nullptr;
    PacketQueue subtitleq;

    double frame_timer = 0;
    double frame_last_returned_time = 0;
    double frame_last_filter_delay = 0;
    int video_stream = -1;
    AVStream* pVideoStream = nullptr;
    PacketQueue videoq;
    double max_frame_duration = 0;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
    struct SwsContext* img_convert_ctx = nullptr;
    /* 字幕信息 */
    struct SwsContext* sub_convert_ctx = nullptr;
    bool eof = false;

    std::string sURL = "";
    SDL_Rect rect{0, 0, 0, 0};
    bool step = false;

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
