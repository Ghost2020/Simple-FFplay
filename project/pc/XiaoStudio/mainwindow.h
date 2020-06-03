#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "FMediaPlayer.h"

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

public slots:
    void onShowMenu();
    void onBeginPlay(bool b);
    void onStopPlay(bool b);
    void onPrevious(bool b);
    void onNext(bool b);
    void onRecord();
    void onQuit();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::MainWindow *ui;

    std::future<bool> m_future;
    QString m_sURL = "";
    QMenu* m_pMenu = nullptr;
    std::shared_ptr<FMediaPlayer> m_pMediaPlayer = nullptr;
};
#endif // MAINWINDOW_H
