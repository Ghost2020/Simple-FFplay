QT       += core gui opengl

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

# set the program icon
#RC_ICONS = $$PWD/ui/res/icon/logo.jpg

# SDL库所需的宏
#DEFINES += SDL_MAIN_HANDLED
DEFINES += _CRT_SECURE_NO_WARNINGS

# 第三方库的路径
THIRD_PARTY_PATH
# ffmpeg 路径
FFMPEG_LIB_PATH
# SDL2 路径
SDL_LIB_PATH

INCLUDEPATH += $$PWD/../../../source/MediaPlayer/include
INCLUDEPATH += $$PWD/source/

win32{
    THIRD_PARTY_PATH = $$PWD/../../../thirdparty
    FFMPEG_LIB_PATH = $$THIRD_PARTY_PATH/ffmpeg/lib/win32
    SDL_LIB_PATH = $$THIRD_PARTY_PATH/SDL2/lib/win32/x64
    INCLUDEPATH += $$THIRD_PARTY_PATH/ffmpeg/include
    INCLUDEPATH += $$THIRD_PARTY_PATH/SDL2/include
    SOURCES += ../../../source/MediaPlayer/src/FMediaPlayer.cpp
}
linux{
    #GTK 库相关
    DEFINES += __GTK_H_INSIDE__    
    QMAKE_CFLAGS += `pkg-config --cflags glib-2.0`
    QMAKE_LIBDIR_FLAGS += `pkg-config --libs glib-2.0`

    THIRD_PARTY_PATH = $$/home/ghostchen/Project/ThirdParty
    FFMPEG_LIB_PATH = $$THIRD_PARTY_PATH/ffmpeg/lib
    INCLUDEPATH += $$THIRD_PARTY_PATH/ffmpeg/include
    #Gtk+ 跨平台的图形工具包
    INCLUDEPATH += /usr/include/gtk-3.0
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
    INCLUDEPATH += /usr/include/pango-1.0/
    INCLUDEPATH += /usr/include/cairo
    INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
    INCLUDEPATH += /usr/include/atk-1.0
    INCLUDEPATH += /usr/include/glib-2.0
    SOURCES += ../../../source/MediaPlayer/src/FMediaPlayer.cpp
}
mac{
    THIRD_PARTY_PATH = $$/usr/local/Cellar
    FFMPEG_LIB_PATH = $$THIRD_PARTY_PATH/ffmpeg/4.2.3_1/lib
    SDL_LIB_PATH = $$THIRD_PARTY_PATH/SDL2/2.0.12_1/lib
    INCLUDEPATH += $$THIRD_PARTY_PATH/ffmpeg/4.2.3_1/include
    INCLUDEPATH += $$THIRD_PARTY_PATH/SDL2/2.0.12_1/include
    SOURCES += ../../../source/MediaPlayer/src/FMediaPlayer.mm
}

LIBS += -L$$FFMPEG_LIB_PATH -lavformat -lavcodec -lavutil -lswresample -lswscale
LIBS += -L$$SDL_LIB_PATH -lSDL2 -lSDL2main
LIBS += -lopengl32 -glu32
linux{
LIBS += -L./usr/lib/x86_64-linux-gnu/ -lz -lm -lvdpau -lva -lx264 -lx265 -lSDL2 -lSDL2main -llzma -lva-drm  -lX11 -lva-x11
}

SOURCES += \
    ../../../source/MediaPlayer/src/FDecoder.cpp \
    main.cpp \
    mainwindow.cpp \
    ui/QSettings.cpp \
    source/QMediaPlayer.cpp

HEADERS += \
    ../../../source/MediaPlayer/include/FMediaPlayer.h \
    mainwindow.h \
    source/QMediaPlayer.h \
    ui/QSettings.h

FORMS += \
    mainwindow.ui \
    ui/QSettings.ui

DISTFILES += \
    ui/res/icon/ic_lock_ringer_off_alpha.png \
    ui/res/icon/ic_lock_ringer_on_alpha.png \
    ui/res/icon/ic_menu_blocked_user.png \
    ui/res/icon/ic_menu_cc_am.png \
    ui/res/icon/ic_menu_delete.png \
    ui/res/icon/ic_settings.png

RESOURCES += \
    ui/Xiao.qrc


