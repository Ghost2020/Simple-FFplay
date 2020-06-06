/**
* \@brief Author			Ghost Chen
* \@brief Email				cxx2020@outlook.com
* \@brief Date				2020/05/20
* \@brief File				QSettings.h
*/
#ifndef QSETTINGS_H
#define QSETTINGS_H

#include <QDialog>

namespace Ui {
class QSettings;
}

class QSettings : public QDialog
{
    Q_OBJECT

public:
    explicit QSettings(QWidget *parent = nullptr);
    ~QSettings();

private:
    Ui::QSettings *ui;
};

#endif // QSETTINGS_H
