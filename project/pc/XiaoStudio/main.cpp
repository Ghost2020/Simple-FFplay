#include "mainwindow.h"

#include <QApplication>

#include <iostream>

using namespace std;

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavutil/avutil.h>
    #include <libavdevice/avdevice.h>
    #include <libavfilter/avfilter.h>
    #include <libavformat/avformat.h>
    #include <libpostproc/postprocess.h>
    #include <libswresample/swresample.h>
    #include <libswscale/swscale.h>
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

