/**
* \@brief Author			Ghost Chen
* \@brief Email				cxx2020@outlook.com
* \@brief Date				2020/05/20
* \@brief File				mainwindow.h
* \@brief Desc:				mainwindow
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QPair>
#include <QList>

#include "QMediaPlayer.h"

#include "ui/QSettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    bool Init();
    void Uninit();

protected:
    void keyPressEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseDoubleClickEvent(QMouseEvent* event) override;

private slots:
    void on_pushButton_Invite_clicked();

    void on_pushButton_Settings_clicked();

private:
    QPoint move_point;
    bool mouse_press;

    Ui::MainWindow *ui = nullptr;

    QSettings* m_pSettings = nullptr;

    /* record the mediaPlayer */
    QList<QPair<int, QMediaPlayer*>> m_String2Screen;

};
#endif // MAINWINDOW_H
