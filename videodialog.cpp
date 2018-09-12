#include "videodialog.h"
#include "ui_videodialog.h"
#include <QtWidgets>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDebug>

VideoDialog::VideoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoDialog)
{
    ui->setupUi(this);

    //
    player = new QMediaPlayer(this);

    playlist = new QMediaPlaylist(this);
    playlist->addMedia(QUrl::fromLocalFile("/opt/vide/vod/a.flv"));
    playlist->addMedia(QUrl::fromLocalFile("/opt/vide/vod/a.flv"));
    //playlist->addMedia(QUrl("http://example.com/myclip2.mp4"));

    //player->setMedia(QUrl::fromLocalFile("/opt/vide/vod/a.flv"));
    videoWidget = new QVideoWidget(this);
    player->setVideoOutput(videoWidget);
    videoWidget->move(300,100);
    videoWidget->resize(320,240);
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    //videoWidget->show();
    playlist->setCurrentIndex(0);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
    player->play();
    qDebug()<<"player";

}

VideoDialog::~VideoDialog()
{
    delete ui;
    if(player->state() == QMediaPlayer::PlayingState)
        player->stop();

     delete playlist;
     delete player;

}

void VideoDialog::on_pushButton_clicked()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle("视频对话clicked");
    msg->show();
}

void VideoDialog::on_pushButton_pressed()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle("视频对话pressed");
    msg->show();
}

void VideoDialog::on_pushButton_3_clicked()
{
    QMessageBox *msg = new QMessageBox(this);
    msg->setWindowTitle("视频对话hold");
    msg->resize(200,100);
    msg->show();
}
