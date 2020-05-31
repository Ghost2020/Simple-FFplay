
#include "FMediaPlayer.h"

extern "C"
{
    #include "cmdutils.h"
}

const char program_name[] = "ffplay";

/* options specified by the user */
static const char* input_filename;

#if CONFIG_AVFILTER
static int opt_add_vfilter(void* optctx, const char* opt, const char* arg)
{
    GROW_ARRAY(vfilters_list, nb_vfilters);
    vfilters_list[nb_vfilters - 1] = arg;
    return 0;
}
#endif

static inline
int cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
    enum AVSampleFormat fmt2, int64_t channel_count2)
{
    /* If channel count == 1, planar and non-planar formats are the same */
    if (channel_count1 == 1 && channel_count2 == 1)
        return av_get_packed_sample_fmt(fmt1) != av_get_packed_sample_fmt(fmt2);
    else
        return channel_count1 != channel_count2 || fmt1 != fmt2;
}

static inline
int64_t get_valid_channel_layout(int64_t channel_layout, int channels)
{
    if (channel_layout && av_get_channel_layout_nb_channels(channel_layout) == channels)
        return channel_layout;
    else
        return 0;
}


static int opt_frame_size(void* optctx, const char* opt, const char* arg)
{
    av_log(nullptr, AV_LOG_WARNING, "Option -s is deprecated, use -video_size.\n");
    return opt_default(nullptr, "video_size", arg);
}

static int opt_frame_pix_fmt(void* optctx, const char* opt, const char* arg)
{
    av_log(nullptr, AV_LOG_WARNING, "Option -pix_fmt is deprecated, use -pixel_format.\n");
    return opt_default(nullptr, "pixel_format", arg);
}

static int opt_show_mode(void* optctx, const char* opt, const char* arg)
{
    FMediaPlayer::eShow_mode = !strcmp(arg, "video") ? FMediaPlayer::EShowMode::SHOW_MODE_VIDEO :
        !strcmp(arg, "waves") ? FMediaPlayer::EShowMode::SHOW_MODE_WAVES :
        !strcmp(arg, "rdft") ? FMediaPlayer::EShowMode::SHOW_MODE_RDFT : FMediaPlayer::EShowMode(int(parse_number_or_die(opt, arg, OPT_INT, 0, double(int(FMediaPlayer::EShowMode::SHOW_MODE_NB) - 1))));
    return 0;
}

static AVInputFormat* file_iformat;

static int opt_format(void* optctx, const char* opt, const char* arg)
{
    file_iformat = av_find_input_format(arg);
    if (!file_iformat) {
        av_log(NULL, AV_LOG_FATAL, "Unknown input format: %s\n", arg);
        return AVERROR(EINVAL);
    }
    return 0;
}

static void opt_input_file(void* optctx, const char* filename)
{
    if (input_filename) {
        av_log(nullptr, AV_LOG_FATAL,
            "Argument '%s' provided as input filename, but '%s' was already specified.\n",
            filename, input_filename);
        exit(1);
    }
    if (!strcmp(filename, "-"))
        filename = "pipe:";
    input_filename = filename;
}

static int dummy;

static const OptionDef options[] = {
    //CMDUTILS_COMMON_OPTIONS
    //{ "x", HAS_ARG, /*{.func_arg = */opt_width/* }*/, "force displayed width", "width" },
    //{ "y", HAS_ARG, /*{.func_arg = */opt_height/* }*/, "force displayed height", "height" },
    { "s", HAS_ARG | OPT_VIDEO,/* {.func_arg = */opt_frame_size/* }*/, "set frame size (WxH or abbreviation)", "size" },
    { "pix_fmt", HAS_ARG | OPT_EXPERT | OPT_VIDEO, /*{.func_arg = */opt_frame_pix_fmt/* }*/, "set pixel format", "format" },
    { "f", HAS_ARG, /*{.func_arg = */opt_format/* }*/, "force format", "fmt" },
    //{ "drp", OPT_INT | HAS_ARG | OPT_EXPERT, { &decoder_reorder_pts }, "let decoder reorder pts 0=off 1=on -1=auto", ""},
    //{ "lowres", OPT_INT | HAS_ARG | OPT_EXPERT, { &lowres }, "", "" },
    //{ "sync", HAS_ARG | OPT_EXPERT,/* {.func_arg = */opt_sync/* }*/, "set audio-video sync. type (type=audio/video/ext)", "type" },
    //{ "autoexit", OPT_BOOL | OPT_EXPERT, { &autoexit }, "exit at the end", "" },
#if CONFIG_AVFILTER
    { "vf", OPT_EXPERT | HAS_ARG, {.func_arg = opt_add_vfilter }, "set video filters", "filter_graph" },
    { "af", OPT_STRING | HAS_ARG, { &afilters }, "set audio filters", "filter_graph" },
#endif
    //{ "rdftspeed", OPT_INT | HAS_ARG | OPT_AUDIO | OPT_EXPERT, { &rdftspeed }, "rdft speed", "msecs" },
    { "showmode", HAS_ARG, /*{.func_arg = */opt_show_mode/*}*/, "select show mode (0 = video, 1 = waves, 2 = RDFT)", "mode" },
    { "default", HAS_ARG | OPT_AUDIO | OPT_VIDEO | OPT_EXPERT, /*{.func_arg = */opt_default/* }*/, "generic catch all option", "" },
    { "i", OPT_BOOL, { &dummy}, "read specified file", "input_file"},
    //{ "autorotate", OPT_BOOL, { &autorotate }, "automatically rotate video", "" },
    //{ "filter_threads", HAS_ARG | OPT_INT | OPT_EXPERT, { &filter_nbthreads }, "number of filter threads per graph" },
    { nullptr, },
};

static void show_usage(void)
{
    av_log(nullptr, AV_LOG_INFO, "Simple media player\n");
    av_log(nullptr, AV_LOG_INFO, "usage: %s [options] input_file\n", program_name);
    av_log(nullptr, AV_LOG_INFO, "\n");
}

void show_help_default(const char* opt, const char* arg)
{
    av_log_set_callback(log_callback_help);
    show_usage();
    show_help_options(options, "Main options:", 0, OPT_EXPERT, 0);
    show_help_options(options, "Advanced options:", OPT_EXPERT, 0, 0);
    printf("\n");
    show_help_children(avcodec_get_class(), AV_OPT_FLAG_DECODING_PARAM);
    show_help_children(avformat_get_class(), AV_OPT_FLAG_DECODING_PARAM);
#if !CONFIG_AVFILTER
    show_help_children(sws_get_class(), AV_OPT_FLAG_ENCODING_PARAM);
#else
    show_help_children(avfilter_get_class(), AV_OPT_FLAG_FILTERING_PARAM);
#endif
    printf("\nWhile playing:\n"
        "q, ESC              quit\n"
        "f                   toggle full screen\n"
        "p, SPC              pause\n"
        "m                   toggle mute\n"
        "9, 0                decrease and increase volume respectively\n"
        "/, *                decrease and increase volume respectively\n"
        "a                   cycle audio channel in the current program\n"
        "v                   cycle video channel\n"
        "t                   cycle subtitle channel in the current program\n"
        "c                   cycle program\n"
        "w                   cycle video filters or show modes\n"
        "s                   activate frame-step mode\n"
        "left/right          seek backward/forward 10 seconds or to custom interval if -seek_interval is set\n"
        "down/up             seek backward/forward 1 minute\n"
        "page down/page up   seek backward/forward 10 minutes\n"
        "right mouse click   seek to percentage in file corresponding to fraction of width\n"
        "left double-click   toggle full screen\n"
    );
}

/*
使用方法如下:
    方式一:本地媒体文件路径

    方式二:直播地址如下:

        湖南卫视 rtmp://58.200.131.2:1935/livetv/hunantv
        广西卫视 ：rtmp://58.200.131.2:1935/livetv/gxtv
        广东卫视：rtmp://58.200.131.2:1935/livetv/gdtv
        东方卫视：rtmp://58.200.131.2:1935/livetv/dftv

    方式三:获取播放设备
        ./ffmpeg -f dshow -list_devices true -i dummy

        本机播放设备: "Logitech HD Webcam C270"

        捕获camera图像信息并播放
        -f dshow -i video="Logitech HD Webcam C270"

*/

/* Called from the main */
int main(int argc, char** argv)
{
    init_dynload();

    parse_loglevel(argc, argv, options);

    std::unique_ptr<FMediaPlayer> pPlayer1 = std::make_unique<FMediaPlayer>();
    //std::unique_ptr<FMediaPlayer> pPlayer2 = std::make_unique<FMediaPlayer>();
    if (pPlayer1 == nullptr)
    {
        av_log(nullptr, AV_LOG_FATAL, "Failed to Create FMediaPlayer\n");
        return -1;
    }

    init_opts();

    signal(SIGINT, FMediaPlayer::sigterm_handler); /* Interrupt (ANSI).    */
    signal(SIGTERM, FMediaPlayer::sigterm_handler); /* Termination (ANSI).  */

    show_banner(argc, argv, options);

    parse_options(nullptr, argc, argv, options, opt_input_file);

    if (!input_filename) 
    {
        show_usage();
        av_log(nullptr, AV_LOG_FATAL, "An input file must be specified\n");
        av_log(nullptr, AV_LOG_FATAL, "Use -h to get full help or, even better, run 'man %s'\n", program_name);
        exit(1);
    }

    if (!pPlayer1->StreamOpen(input_filename, file_iformat))
    {
        av_log(nullptr, AV_LOG_FATAL, "Failed to Open stream\n");
        return -1;
    }

    /*if (!pPlayer2->stream_open("F:/CXX/file/video/GB28181视频教程第一集（基础知识）.flv"))
    {
        av_log(nullptr, AV_LOG_FATAL, "Failed to Open stream\n");
        return -1;
    }*/

    //std::this_thread::sleep_for(std::chrono::seconds(2));

    pPlayer1->EventLoop();
    
    /* TODO #SEQ1 */
    /*std::function<void()> func1 = std::bind(&FMediaPlayer::event_loop, &(*pPlayer1));
    std::future<void> fut1 = std::async(std::launch::async, std::move(func1));*/

    /*std::function<void()> func2 = std::bind(&FMediaPlayer::event_loop, &(*pPlayer2));
    std::future<void> fut2 = std::async(std::launch::deferred, std::move(func2));*/

    //fut1.get();
    //fut2.get();

    uninit_opts();

    /* never returns */
    return 0;
}
