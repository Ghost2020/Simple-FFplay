#include "QMediaPlayer.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QMimeData> 
#include <QFileInfo>
#include <iostream>

#if defined(Q_OS_MACOS)
//#import<cocoa/cocoa.h>
#import <Appkit/NSWindow.h>
#endif

QMediaPlayer::QMediaPlayer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

    /* Set Accpet drop event */
    this->setAcceptDrops(true);
    this->setMouseTracking(true);

    /* Set the widget palatee */

    this->m_pMenu = new QMenu(this);
    QAction *beginPlayAction = this->m_pMenu->addAction(QString("Play"));
    QAction* pausePlayAction = this->m_pMenu->addAction(QString("Pause"));
    pausePlayAction->setCheckable(true);
    QAction* AudioPlayAction = this->m_pMenu->addAction(QString("Audio"));
    AudioPlayAction->setCheckable(true);
    QAction *stopPlayAction = this->m_pMenu->addAction(QString("Stop"));
    QAction *recordAction = this->m_pMenu->addAction(QString("Record"));
    recordAction->setCheckable(true);
    this->m_pMenu->addSeparator();
    QAction *quitAction = this->m_pMenu->addAction(QString("Quit"));

    connect(beginPlayAction, SIGNAL(triggered(bool)), this, SLOT(onBeginPlay()));
    connect(pausePlayAction, SIGNAL(triggered(bool)), this, SLOT(OnPausePlay()));
    connect(AudioPlayAction, SIGNAL(triggered(bool)), this, SLOT(OnToggleMute()));
    connect(stopPlayAction, SIGNAL(triggered(bool)), this, SLOT(onStopPlay()));
    connect(recordAction, SIGNAL(triggered(bool)), this, SLOT(onRecord()));
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(onQuit()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onShowMenu()));

    this->m_pTimer = new QTimer(this);
    this->m_pTimer->setInterval(20);
    connect(this->m_pTimer, SIGNAL(timeout()), this, SLOT(ON_TEST()));

    /*this->m_pProgressSlider = new QSlider(this);
    this->m_pProgressSlider->setOrientation(Qt::Orientation::Horizontal);
    this->m_pProgressSlider->showNormal();*/
}

QMediaPlayer::~QMediaPlayer()
{
    onStopPlay();
}

void QMediaPlayer::openStream()
{
    onStopPlay();

#if defined(Q_OS_WIN32)
    this->m_pCorePlayer = std::make_unique<FMediaPlayer>(HWND(this->winId()));
#elif defined(Q_OS_MACOS)
    NSView* view = (NSView*)this->effectiveWinId();
    NSWindow* wnd = [view window];
    this->m_pCorePlayer = std::make_unique<FMediaPlayer>(static_cast<void*>(wnd));
#elif defined(Q_OS_LINUX)
    ;
#endif

    if (!this->m_pCorePlayer->OnStreamOpen(this->m_sURL.toStdString()))
    {
        QMessageBox::warning(this, "Warning", "Can't open this media file!");
        this->m_pCorePlayer = nullptr;
        this->m_sURL = "";
    }
    else
    {
        if (this->m_pTimer->isActive())
            this->m_pTimer->stop();

        this->m_pTimer->start();

        /* Need to force refresh */
        resizeEvent(nullptr);
    }
}

void QMediaPlayer::onShowMenu()
{
    this->m_pMenu->exec(QCursor::pos());
}

void QMediaPlayer::onBeginPlay()
{
    this->m_sURL = QFileDialog::getOpenFileName();

    if(!this->m_sURL.isEmpty())
        openStream();
}

void QMediaPlayer::OnPausePlay()
{
    if (this->m_pCorePlayer)
        this->m_pCorePlayer->OnTogglePause();
}

void QMediaPlayer::OnToggleMute()
{
    if (this->m_pCorePlayer)
        this->m_pCorePlayer->OnToggleMute();
}

void QMediaPlayer::onStopPlay()
{
    if(this->m_pTimer->isActive())
        this->m_pTimer->stop();

    if(this->m_pCorePlayer)
        this->m_pCorePlayer = nullptr;
}

void QMediaPlayer::onRecord()
{
    QMessageBox::information(this, "onRecord", "Record");
}

void QMediaPlayer::onQuit()
{
    QMessageBox::information(this, "onQuit", "Quit");
}

void QMediaPlayer::ON_TEST()
{
    SDL_Event event;
    if(this->m_pCorePlayer)
        this->m_pCorePlayer->refresh_loop_wait_event(event);
}

void QMediaPlayer::keyPressEvent(QKeyEvent* event)
{
    double incr = 0, pos = 0;

    if (this->m_pCorePlayer)
    {
        switch (event->key())
        {
        case Qt::Key_F:
            this->m_pCorePlayer->OnToggleFullScreen();
            break;
        case Qt::Key_Escape:
            this->m_pCorePlayer->StreamClose();
            break;
        case Qt::Key_P:
        case Qt::Key_Space:
            this->m_pCorePlayer->OnTogglePause();
            break;
        case Qt::Key_M:
            this->m_pCorePlayer->OnToggleMute();
            break;
        case Qt::Key_Minus:
            this->m_pCorePlayer->OnUpdateVolume(-1, SDL_VOLUME_STEP);
            break;
        case Qt::Key_Plus:
            this->m_pCorePlayer->OnUpdateVolume(1, SDL_VOLUME_STEP);
            break;
        case Qt::Key_S:
            this->m_pCorePlayer->OnStepToNextFrame();
            break;
        case Qt::Key_A:
            this->m_pCorePlayer->OnStreamCycleChannel(AVMEDIA_TYPE_AUDIO);
            break;
        case Qt::Key_V:
            this->m_pCorePlayer->OnStreamCycleChannel(AVMEDIA_TYPE_VIDEO);
            break;
        case Qt::Key_T:
            this->m_pCorePlayer->OnStreamCycleChannel(AVMEDIA_TYPE_SUBTITLE);
            break;
        case Qt::Key_C:
            this->m_pCorePlayer->OnStreamCycleChannel(AVMEDIA_TYPE_VIDEO);
            this->m_pCorePlayer->OnStreamCycleChannel(AVMEDIA_TYPE_AUDIO);
            this->m_pCorePlayer->OnStreamCycleChannel(AVMEDIA_TYPE_SUBTITLE);
            break;
        case Qt::Key_PageUp:
            if (this->m_pCorePlayer->pFormatCtx->nb_chapters <= 1)
            {
                incr = 600.0;
                goto do_seek;
            }
            this->m_pCorePlayer->OnSeekChapter(1);
            break;
        case Qt::Key_PageDown:
            if (this->m_pCorePlayer->pFormatCtx->nb_chapters <= 1)
            {
                incr = -600.0;
                goto do_seek;
            }
            this->m_pCorePlayer->OnSeekChapter(-1);
            break;
        case Qt::Key_Left:
            incr = this->m_pCorePlayer->seek_interval ? -this->m_pCorePlayer->seek_interval : -10.0;
            goto do_seek;
        case Qt::Key_Right:
            incr = this->m_pCorePlayer->seek_interval ? this->m_pCorePlayer->seek_interval : 10.0;
            goto do_seek;
        case Qt::Key_Up:
            incr = 60.0;
            goto do_seek;
        case Qt::Key_Down:
            incr = -60.0;
        do_seek:
            if (this->m_pCorePlayer->seek_by_bytes) {
                pos = -1;
                if (pos < 0 && this->m_pCorePlayer->video_stream >= 0)
                    pos = this->m_pCorePlayer->pictq.LastPos();
                if (pos < 0 && this->m_pCorePlayer->audio_stream >= 0)
                    pos = this->m_pCorePlayer->sampq.LastPos();
                if (pos < 0)
                    pos = avio_tell(this->m_pCorePlayer->pFormatCtx->pb);
                if (this->m_pCorePlayer->pFormatCtx->bit_rate)
                    incr *= this->m_pCorePlayer->pFormatCtx->bit_rate / 8.0;
                else
                    incr *= 180000.0;
                pos += incr;
                this->m_pCorePlayer->OnStreamSeek(int64_t(pos), int64_t(incr), true);
            }
            else {
                pos = this->m_pCorePlayer->get_master_clock();
                if (isnan(pos))
                    pos = double(this->m_pCorePlayer->seek_pos / AV_TIME_BASE);
                pos += incr;
                if (this->m_pCorePlayer->pFormatCtx->start_time != AV_NOPTS_VALUE && pos < this->m_pCorePlayer->pFormatCtx->start_time / (double)AV_TIME_BASE)
                    pos = this->m_pCorePlayer->pFormatCtx->start_time / double(AV_TIME_BASE);
                this->m_pCorePlayer->OnStreamSeek(int64_t(pos * AV_TIME_BASE), int64_t(incr * AV_TIME_BASE), false);
            }
            break;
        }
    }

    QOpenGLWidget::keyPressEvent(event);
}

void QMediaPlayer::mousePressEvent(QMouseEvent *event)
{
    if (this->m_pCorePlayer)
    {
        if ((event->type() == QEvent::MouseButtonDblClick) /*&&*/
            /*(event->button()  == Qt::MouseButton::LeftButton)*/)
        {
            if (av_gettime_relative() - m_nLast_mouse_left_click <= 500000) {
                this->m_pCorePlayer->OnToggleFullScreen();
                m_nLast_mouse_left_click = 0;
            }
            else
                m_nLast_mouse_left_click = av_gettime_relative();
        }

        event->accept();
    }

    event->ignore();
    //QOpenGLWidget::mousePressEvent(event);
}

void QMediaPlayer::mouseMoveEvent(QMouseEvent* event)
{
    if (this->m_pCorePlayer)
    {
       /* if (this->m_pCorePlayer->cursor_hidden)
        {
            SDL_ShowCursor(1);
            this->m_pCorePlayer->cursor_hidden = false;
        }
        this->m_pCorePlayer->cursor_last_shown = av_gettime_relative();
        double x = 0;

        if (this->m_pCorePlayer->pFormatCtx)
        {
            if (this->m_pCorePlayer->seek_by_bytes || this->m_pCorePlayer->pFormatCtx->duration <= 0) {
                uint64_t size = avio_size(this->m_pCorePlayer->pFormatCtx->pb);
                this->m_pCorePlayer->OnStreamSeek(size * x / this->m_pCorePlayer->rect.w, 0, true);
            }
            else {
                const int tns = this->m_pCorePlayer->pFormatCtx->duration / 1000000LL;
                const int thh = tns / 3600;
                const int tmm = (tns % 3600) / 60;
                const int tss = (tns % 60);
                const int frac = x / this->m_pCorePlayer->rect.w;
                const int ns = frac * tns;
                const int hh = ns / 3600;
                const int mm = (ns % 3600) / 60;
                const int ss = (ns % 60);
                av_log(nullptr, AV_LOG_INFO, "Seek to %2.0f%% (%2d:%02d:%02d) of total duration (%2d:%02d:%02d)       \n", frac * 100,
                    hh, mm, ss, thh, tmm, tss);
                int64_t ts = frac * this->m_pCorePlayer->pFormatCtx->duration;
                if (this->m_pCorePlayer->pFormatCtx->start_time != AV_NOPTS_VALUE)
                    ts += this->m_pCorePlayer->pFormatCtx->start_time;
                this->m_pCorePlayer->OnStreamSeek(ts, 0, false);
            }
        }*/
    }
    //QOpenGLWidget::mouseMoveEvent(event);
}

void QMediaPlayer::resizeEvent(QResizeEvent *event)
{
    /* resize the Canvas */
    if (this->m_pCorePlayer)
    {
        this->m_pCorePlayer->screen_width = uint16_t(this->m_pCorePlayer->rect.w = size().width());
        this->m_pCorePlayer->screen_height = uint16_t(this->m_pCorePlayer->rect.h = size().height());
        if (this->m_pCorePlayer->vis_texture) 
        {
            SDL_DestroyTexture(this->m_pCorePlayer->vis_texture);
            this->m_pCorePlayer->vis_texture = nullptr;
        }
    }

    /* resize the slider */
   /* QSize size = this->size();
    this->m_pProgressSlider->setGeometry(5, size.rheight() - 20, size.rwidth() - 10, 20);
    this->m_pProgressSlider->show();*/

    if(event)
        QOpenGLWidget::resizeEvent(event);
}

void QMediaPlayer::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void QMediaPlayer::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        event->ignore();
 
    this->m_sURL = urls[0].toLocalFile();
    openStream();

    event->accept();
}
