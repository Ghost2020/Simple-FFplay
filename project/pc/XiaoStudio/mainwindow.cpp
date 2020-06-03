#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::Init()
{
#pragma region menu
    m_pMenu = new QMenu();
    QAction *beginPlayAction = m_pMenu->addAction(QString("Play"));
    QAction *stopPlayAction = m_pMenu->addAction(QString("Stop"));
    QAction *previousAction = m_pMenu->addAction(QString("Previous"));
    QAction *nextAction = m_pMenu->addAction(QString("Next"));
    QAction *recordAction = m_pMenu->addAction(QString("Record"));
    m_pMenu->addSeparator();
    QAction *quitAction = m_pMenu->addAction(QString("Quit"));

    connect(beginPlayAction, SIGNAL(triggered(bool)), this, SLOT(onBeginPlay(bool)));
    connect(stopPlayAction, SIGNAL(triggered(bool)), this, SLOT(onStopPlay(bool)));
    connect(previousAction, SIGNAL(triggered(bool)), this, SLOT(onPrevious(bool)));
    connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(onNext(bool)));
    connect(recordAction, SIGNAL(triggered(bool)), this, SLOT(onRecord()));
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(onQuit()));

    ui->screen->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->screen, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onShowMenu()));
#pragma endregion menu

    this->m_pMediaPlayer = std::make_shared<FMediaPlayer>();

    return true;
}

void MainWindow::Uninit()
{

}

void MainWindow::onBeginPlay(bool b)
{
    m_sURL = QFileDialog::getOpenFileName();

    if(!m_pMediaPlayer->OnStreamOpen(m_sURL.toStdString()))
    {
        QMessageBox::warning(this, "Warning", "Can't open this media file!");
    }
    else
    {
        m_future = std::async(std::launch::async, [&] () -> bool
        {
            double remaining_time = 0.0;
            while(!m_pMediaPlayer->abort_request)
            {
                std::cout << "Loop" << std::endl;
                if (!m_pMediaPlayer->cursor_hidden && av_gettime_relative() - m_pMediaPlayer->cursor_last_shown > CURSOR_HIDE_DELAY)
                {
                    m_pMediaPlayer->cursor_hidden = 1;
                }
                if (remaining_time > 0.0)
                    std::this_thread::sleep_for(std::chrono::microseconds((int64_t)(remaining_time * 1000000.0)));
                remaining_time = REFRESH_RATE;
                if (m_pMediaPlayer->eShow_mode != FMediaPlayer::EShowMode::SHOW_MODE_NONE && (!m_pMediaPlayer->paused || m_pMediaPlayer->force_refresh))
                    m_pMediaPlayer->video_refresh(remaining_time);
            }

            return true;
        });
    }
}

void MainWindow::onStopPlay(bool b)
{
    QMessageBox::information(this, "onStopPlay", "onStopPlay");
}

void MainWindow::onPrevious(bool b)
{
    QMessageBox::information(this, "onPrevious", "onPrevious");
}

void MainWindow::onNext(bool b)
{
    QMessageBox::information(this, "onNext", "onNext");
}

void MainWindow::onRecord()
{
    QMessageBox::information(this, "onRecord", "onRecord");
}

void MainWindow::onQuit()
{
    this->close();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(m_pMediaPlayer == nullptr)
        QMainWindow::keyPressEvent(event);

    double incr = 0, pos = 0;

    std::cout << "keyPressEvent" << std::endl;

    switch(event->key())
    {
        case Qt::Key_F :
            m_pMediaPlayer->OnToggleFullScreen();
            break;
        case Qt::Key_Escape :
            m_pMediaPlayer->StreamClose();
            break;
        case Qt::Key_P :
        case Qt::Key_Space:
            m_pMediaPlayer->OnTogglePause();
            break;
        case Qt::Key_M:
            m_pMediaPlayer->OnToggleMute();
            break;
        case Qt::Key_Minus:
            m_pMediaPlayer->OnUpdateVolume(-1, SDL_VOLUME_STEP);
            break;
        case Qt::Key_Plus:
            m_pMediaPlayer->OnUpdateVolume( 1, SDL_VOLUME_STEP);
            break;
        case Qt::Key_S:
            m_pMediaPlayer->OnStepToNextFrame();
            break;
        case Qt::Key_A:
            m_pMediaPlayer->OnStreamCycleChannel(AVMEDIA_TYPE_AUDIO);
            break;
        case Qt::Key_V:
            m_pMediaPlayer->OnStreamCycleChannel(AVMEDIA_TYPE_VIDEO);
            break;
        case Qt::Key_T:
            m_pMediaPlayer->OnStreamCycleChannel(AVMEDIA_TYPE_SUBTITLE);
            break;
        case Qt::Key_C:
            m_pMediaPlayer->OnStreamCycleChannel(AVMEDIA_TYPE_VIDEO);
            m_pMediaPlayer->OnStreamCycleChannel(AVMEDIA_TYPE_AUDIO);
            m_pMediaPlayer->OnStreamCycleChannel(AVMEDIA_TYPE_SUBTITLE);
            break;
        case Qt::Key_PageUp:
            if(m_pMediaPlayer->pFormatCtx->nb_chapters <= 1)
            {
                incr = 600.0;
                goto do_seek;
            }
            m_pMediaPlayer->OnSeekChapter(1);
            break;
        case Qt::Key_PageDown:
            if (m_pMediaPlayer->pFormatCtx->nb_chapters <= 1)
            {
                incr = -600.0;
                goto do_seek;
            }
            m_pMediaPlayer->OnSeekChapter(-1);
            break;
        case Qt::Key_Left:
            incr = m_pMediaPlayer->seek_interval ? -m_pMediaPlayer->seek_interval : -10.0;
            goto do_seek;
        case Qt::Key_Right:
            incr = m_pMediaPlayer->seek_interval ? m_pMediaPlayer->seek_interval : 10.0;
            goto do_seek;
        case Qt::Key_Up:
            incr = 60.0;
            goto do_seek;
        case Qt::Key_Down:
            incr = -60.0;
            do_seek:
            if (m_pMediaPlayer->seek_by_bytes) {
                pos = -1;
                if (pos < 0 && m_pMediaPlayer->video_stream >= 0)
                    pos = m_pMediaPlayer->pictq.LastPos();
                if (pos < 0 && m_pMediaPlayer->audio_stream >= 0)
                    pos = m_pMediaPlayer->sampq.LastPos();
                if (pos < 0)
                    pos = avio_tell(m_pMediaPlayer->pFormatCtx->pb);
                if (m_pMediaPlayer->pFormatCtx->bit_rate)
                    incr *= m_pMediaPlayer->pFormatCtx->bit_rate / 8.0;
                else
                    incr *= 180000.0;
                pos += incr;
                m_pMediaPlayer->OnStreamSeek(pos, incr, true);
            }
            else {
                pos = m_pMediaPlayer->get_master_clock();
                if (isnan(pos))
                    pos = (double)m_pMediaPlayer->seek_pos / AV_TIME_BASE;
                pos += incr;
                if (m_pMediaPlayer->pFormatCtx->start_time != AV_NOPTS_VALUE && pos < m_pMediaPlayer->pFormatCtx->start_time / (double)AV_TIME_BASE)
                    pos = m_pMediaPlayer->pFormatCtx->start_time / (double)AV_TIME_BASE;
                m_pMediaPlayer->OnStreamSeek((int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), false);
            }
            break;
        default:
            QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::onShowMenu()
{
    m_pMenu->exec(QCursor::pos());
}

