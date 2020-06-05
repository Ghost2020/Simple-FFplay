#include "QSettings.h"
#include "ui_QSettings.h"

QSettings::QSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSettings)
{
    ui->setupUi(this);
}

QSettings::~QSettings()
{
    delete ui;
}
