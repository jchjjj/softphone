#ifndef VIDEODIALOG_H
#define VIDEODIALOG_H

#include <QDialog>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
namespace Ui {
class VideoDialog;
}

class VideoDialog : public QDialog
{
    Q_OBJECT



public:
    explicit VideoDialog(QWidget *parent = 0);
    ~VideoDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_pressed();

    void on_pushButton_3_clicked();

private:
    Ui::VideoDialog *ui;

    QMediaPlayer * player;
    QMediaPlaylist * playlist;
    QVideoWidget * videoWidget;
};

#endif // VIDEODIALOG_H
