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
