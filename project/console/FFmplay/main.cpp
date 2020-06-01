
#include "FMediaPlayer.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void show_usage(void)
{
    av_log(nullptr, AV_LOG_INFO, "Simple media player\n");
    av_log(nullptr, AV_LOG_INFO, "usage: %s [options] input_file\n", "FFmplay");
    av_log(nullptr, AV_LOG_INFO, "\n");
}

void init_dynload(void)
{
#ifdef _WIN32
    /* Calling SetDllDirectory with the empty string (but not NULL) removes the
     * current working directory from the DLL search path as a security pre-caution. */
    SetDllDirectory(L"");
#endif
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
    if (argc < 1)
    {
        show_usage();
        return -1;
    }
        
    const std::string input_filename = argv[1];

    if (input_filename.empty())
    {
        show_usage();
        av_log(nullptr, AV_LOG_FATAL, "An input file must be specified\n");
        av_log(nullptr, AV_LOG_FATAL, "Use -h to get full help or, even better, run 'man %s'\n", "FFmplay");
        exit(1);
    }

    init_dynload();

    std::unique_ptr<FMediaPlayer> pPlayer1 = std::make_unique<FMediaPlayer>();
    if (pPlayer1 == nullptr)
    {
        av_log(nullptr, AV_LOG_FATAL, "Failed to Create FMediaPlayer\n");
        return -1;
    }

    signal(SIGINT, FMediaPlayer::sigterm_handler); /* Interrupt (ANSI).    */
    signal(SIGTERM, FMediaPlayer::sigterm_handler); /* Termination (ANSI).  */

    if (!pPlayer1->OnStreamOpen(input_filename))
    {
        av_log(nullptr, AV_LOG_FATAL, "Failed to Open stream\n");
        return -1;
    }

    /* TODO #SEQ1 */
    for (;;)
    {
        pPlayer1->OnTick();
    }

    /* never returns */
    return 0;
}
