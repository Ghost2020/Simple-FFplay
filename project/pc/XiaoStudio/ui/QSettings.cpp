#include "QSettings.h"
#include "ui_QSettings.h"

#include <QDesktopServices>

#include <string>
#include <iostream>

QSettings::QSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSettings)
{
    ui->setupUi(this);

    this->setWindowTitle("Settings");

    /* 名称设置 */
    this->ui->listWidget->item(0)->setText("常规设置");
    this->ui->listWidget->item(1)->setText("音频设置");
    this->ui->listWidget->item(2)->setText("视频设置");
    this->ui->listWidget->item(3)->setText("虚拟背景");
    this->ui->listWidget->item(4)->setText("网络测试");
    this->ui->listWidget->item(5)->setText("关于我们");

    /* 建立listWidget与 stackedWidget的映射 */
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

    /* 网页打开 */
    connect(ui->pushButton_github, &QPushButton::clicked, this, [] () -> void{
         QDesktopServices::openUrl (QUrl("https://github.com/Ghost2020"));
    });

    /* 话筒音量 */
    connect(ui->horizontalSlider_volume, &QSlider::valueChanged, this, [=] (int value) -> void{
        std::cout << "Speaker volume changed::" << value << std::endl;
    });

    /* 喇叭音量 */
    connect(ui->horizontalSlider_volume_Microphone, &QSlider::valueChanged, this, [=] (int value) -> void{
        std::cout << "Microphone volume changed::" << value << std::endl;
    });

    /* */

    /* 网络测试 */
}

QSettings::~QSettings()
{
    delete ui;
}
