
#include "FMediaPlayer.h"

#include <iostream>

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
*读取摄像头数据，并播放
使用接口 av_find_input_format
* /
//Winows
//{
//  设备列表获取 = ./ffmpeg -f dshow -list_devices true -i dummy
//    videoCapture = "Logitech HD Webcam C270"
//    audioDevice = "DirectShow audio devices"

//    摄像头采集播放 = ./ffplay -f dshow -i video="Logitech HD Webcam C270"
//}
//Linux
/*{
    设备列表获取 = ./ffmpeg -hided_banner -devices
    摄像头采集播放 =
}*/
//Mac
/*{
    设备列表获取 = ./ffmpeg -f avfoundation -list_edvices true -i ""
    摄像头采集播放 = ./ffplay -f avfoundatin -video_size 640x480 -framerate 30 -i "视频采集设备名称"
}*/

/*
    直播地址
*/
/*
    湖南卫视: rtmp://58.200.131.2:1935/livetv/hunantv
    广西卫视：rtmp://58.200.131.2:1935/livetv/gxtv
    广东卫视：rtmp://58.200.131.2:1935/livetv/gdtv
    东方卫视：rtmp://58.200.131.2:1935/livetv/dftv
*/

/* Called from the main */
int main(int argc, char** argv)
{
    if (argc < 1)
    {
        show_usage();
        return -1;
    }

    for (int i = 0; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
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

    signal(SIGINT, FMediaPlayer::sigterm_handler);  /* Interrupt (ANSI).    */
    signal(SIGTERM, FMediaPlayer::sigterm_handler); /* Termination (ANSI).  */

    AVInputFormat* iformat = nullptr;
    if (std::string(argv[1]) == "-f")
    {
        iformat = av_find_input_format("dshow");
    }

    if (!pPlayer1->OnStreamOpen(input_filename))
    {
        av_log(nullptr, AV_LOG_FATAL, "Failed to Open stream\n");
        return -1;
    }

    /* TODO #SEQ1 */
    for (;;)
    {
        if (!pPlayer1->OnTick())
            break;
    }

    /* never returns */
    return 0;
}
