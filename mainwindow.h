#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "sipthread.h"
#include <pjsua2.hpp>
#include <QLabel>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum AUDIOINDEX{
        RING =0,
        NEWMESSAGE,
        DIAL,
        BUSY
        };

    inline void play(AUDIOINDEX index, QMediaPlaylist::PlaybackMode mode){
        audiolist->setCurrentIndex(index);
        audiolist->setPlaybackMode(mode);
        //player->setVolume(100);
        player->play();
    }


    // my functions -- jch

signals:
    void makecall(QString);
    void hang(QString id);
    void answer(QString id,bool yesorno);
    void sendmsg(QString id, QString text);

public slots:
    void about();
    void quit();
    void showSettingsWin();
    void addNum(QString num);
    void showVideo(bool);
    void setupSip();
    void startSip();
    void call();
    void hangup();
    void im();
    void readFromSip();
    void incommingCall(QString remoteUri);
    void recvmsg(const OnInstantMessageParam & prm);
private slots:

    void on_backspaceButton_clicked();
    void on_pushButton_call_clicked();
    void on_pushButton_hang_clicked();

    void on_pushButton_msg_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
    QLabel statusLabel;\

    QMediaPlayer * player;
    QMediaPlaylist * audiolist;
//    QProcess sipProcess;
    //SipThread * sipthread;
};

#endif // MAINWINDOW_H
