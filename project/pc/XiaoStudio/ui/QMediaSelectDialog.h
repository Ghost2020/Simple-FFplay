#ifndef QMEIDASELECTDIALOG_H
#define QMEIDASELECTDIALOG_H

#include <QDialog>

namespace Ui {
class QMeidaSelectDialog;
}

class QMeidaSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QMeidaSelectDialog(QWidget *parent = nullptr);
    ~QMeidaSelectDialog();

private:
    Ui::QMeidaSelectDialog *ui;
};

#endif // QMEIDASELECTDIALOG_H
