#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStatusBar>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* no border */
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);

    ui->gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
    this->setMouseTracking(true);
    this->setWindowTitle("XiaoStudio");
    this->statusBar()->setHidden(true);

    this->m_pSettings = new QSettings(this);

    connect(ui->pushButton_Minmum, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));
    connect(ui->pushButton_Maxmum, SIGNAL(clicked(bool)), this, SLOT(showMaximized()));
    connect(ui->pushButton_Exit, SIGNAL(clicked(bool)), this, SLOT(close()));
   
    ui->pushButton_Minmum->setStyleSheet("QPushButton{background-color:black;\
        color: white;   border - radius: 10px;  border: 2px groove gray; \
        border - style: outset;}" \
        "QPushButton:hover{background-color:white; color: black;}"
        "QPushButton:pressed{background-color:rgb(85, 170, 255);\
        border - style: inset; }");
    ui->pushButton_Maxmum->setStyleSheet("QPushButton{background-color:black;\
        color: white;   border - radius: 10px;  border: 2px groove gray; \
        border - style: outset;}" \
        "QPushButton:hover{background-color:white; color: black;}"
        "QPushButton:pressed{background-color:rgb(85, 170, 255);\
        border - style: inset; }");
    ui->pushButton_Exit->setStyleSheet("QPushButton{background-color:black;\
        color: white;   border - radius: 10px;  border: 2px groove gray; \
        border - style: outset;}" \
        "QPushButton:hover{ background-color:red; color: black;}"
        "QPushButton:pressed{background-color:rgb(85, 170, 255);\
        border - style: inset; }");
    //ui->horizontalLayout_MainBar->set
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::Init()
{
    return true;
}

void MainWindow::Uninit()
{

}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
        case Qt::Key::Key_F11:
        {
            if (!this->isMaximized())
            {
                this->showMaximized();
            }
            else
            {
                this->showNormal();
            }
        }
        break;
    default:
        ;
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    /*int y1 = event->globalPos().y();
    int y2 = this->pos().y();*/
    auto pos1 = event->localPos();
    auto pos2 = event->windowPos();
    auto pos3 = event->screenPos();
    if ( (event->button() == Qt::LeftButton) ||
         (event->pos().y() < 50) )
    {
        mouse_press = true;

        //鼠标相对于窗体的位置（或者使用 event->globalPos() - this->pos()）
        move_point = event->pos();

    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    mouse_press = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (mouse_press)
    {
        QPoint move_pos = event->globalPos();

        this->move(move_pos - move_point);
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!this->isFullScreen())
            this->showFullScreen();
        else
            this->showNormal();
    }
}

void MainWindow::on_pushButton_Invite_clicked()
{
    const auto rowCount = ui->gridLayout->rowCount();
    const auto colCount = ui->gridLayout->columnCount();

    int row = 0, col = 0;
    int lastRow = 0, lastCol = 0;
    if (m_String2Screen.size() >= 1)
    {
        const auto& item = m_String2Screen.last();
        lastRow = item.first / 10;
        lastCol = item.first % 10;
    }
    
    if (rowCount == colCount)
    {
        if (lastRow == lastCol)
        {
            row = 0;
            col = lastCol + 1;
        }
        else
        {
            row = lastRow;
            col = lastCol + 1;
        }
    }
    else if (rowCount < colCount)
    {
        if (lastRow < lastCol && (lastRow + 1) == lastCol)
        {
            row = lastRow + 1;
            col = 0;
        }
        else
        {
            row = lastRow + 1 ;
            col = lastCol;
        }
    }

    QMediaPlayer* screen = new QMediaPlayer(ui->centralwidget);
    const int screenSeq = row * 10 + col;
    screen->setObjectName(std::to_string(screenSeq).c_str());
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(screen->sizePolicy().hasHeightForWidth());
    screen->setSizePolicy(sizePolicy);

    ui->gridLayout->addWidget(screen, row, col);

    m_String2Screen.push_back(qMakePair<int, QMediaPlayer*>(screenSeq, screen));
}

void MainWindow::on_pushButton_Settings_clicked()
{
    //m_pSettings->showNormal();
    m_pSettings->setModal(true);
    m_pSettings->showNormal();
}
