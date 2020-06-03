QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# SDL库所需的宏
DEFINES += SDL_MAIN_HANDLED
DEFINES += _CRT_SECURE_NO_WARNINGS

INCLUDEPATH += $$PWD/../../../thirdparty/ffmpeg/include
INCLUDEPATH += $$PWD/../../../thirdparty/SDL2-2.0.12/include
INCLUDEPATH += $$PWD/../../../source/MediaPlayer/include

win32{
    LIBS += -L$$PWD/../../../thirdparty/ffmpeg/lib/win32 -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale
    LIBS += -L$$PWD/../../../thirdparty/SDL2-2.0.12/lib/win32/x64 -lSDL2 #-lSDL2main
}
unix{

}
mac{

}

SOURCES += \
    ../../../source/MediaPlayer/src/FDecoder.cpp \
    ../../../source/MediaPlayer/src/FMediaPlayer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../../../source/MediaPlayer/include/FDecoder.h \
    ../../../source/MediaPlayer/include/FMediaPlayer.h \
    mainwindow.h

FORMS += \
    mainwindow.ui


