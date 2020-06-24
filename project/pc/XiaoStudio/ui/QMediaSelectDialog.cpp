#include "qmeidaselectdialog.h"
#include "ui_qmeidaselectdialog.h"

QMeidaSelectDialog::QMeidaSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QMeidaSelectDialog)
{
    ui->setupUi(this);
}

QMeidaSelectDialog::~QMeidaSelectDialog()
{
    delete ui;
}
