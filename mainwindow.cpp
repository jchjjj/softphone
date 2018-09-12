#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_chatwindow.h"
#include "ui_settingsdialog.h"
#include "ui_videodialog.h"
#include "chatwindow.h"
#include "settingsdialog.h"
#include "videodialog.h"

#include <QDebug>

#include <QtGui>

#include <QtWidgets>
#include <QProcess>
#include <QTimer>
#include <model/config.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->addWidget(&statusLabel);

    //set ui style
    setWindowFlags(windowFlags()&
                     ~Qt::WindowMaximizeButtonHint&
                     ~Qt::WindowMinimizeButtonHint);    //窗体没有最大化最小化按钮
    setFixedSize(this->width(),this->height());

    //signal and slot
    QList<NumberKey*> keys = this->findChildren<NumberKey*>();
    for(auto key :keys){
        connect(key,SIGNAL(numPressed(QString)),this,SLOT(addNum(QString)));
    }
    connect(ui->action_quit,SIGNAL(triggered()),this,SLOT(quit()) );
    connect(ui->action_about,SIGNAL(triggered()),this,SLOT(about()) );

    connect(ui->action_video,SIGNAL(triggered(bool)),this,SLOT(showVideo(bool)));
    connect(ui->action_addBuddy,SIGNAL(triggered()),this,SLOT(im()) );
    connect(ui->action_preference,SIGNAL(triggered()),this,SLOT(showSettingsWin()));
    //
    //connect(&sipProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(readFromSip()));
    //timer
//    QTimer * timer = new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(startSip()));
//    timer->setSingleShot(true);
//    timer->start(300 );

    // register thread to pjsip
//    Endpoint& ep = Global::instance()->manager->ep;
//    ep.libRegisterThread("uithread");
    qDebug()<<"mainwindow UI thread: "<< thread();
    qDebug() <<"MainWindow::MainWindow(QWidget *parent) :" <<  QThread::currentThread();

    // media
    player = new QMediaPlayer(this);
    audiolist = new QMediaPlaylist(this);
    QDir dir("sounds");
//    dir.setFilter(QDir::Files);
//    for(QString file
    qDebug()<< QUrl::fromLocalFile(dir.absoluteFilePath("ring.wav"));
    audiolist->addMedia(QUrl::fromLocalFile(dir.absoluteFilePath("ring.wav")));
    audiolist->addMedia(QUrl::fromLocalFile(dir.absoluteFilePath("newmessage.wav")));
    audiolist->addMedia(QUrl::fromLocalFile(dir.absoluteFilePath("dialtone.wav")));
    audiolist->addMedia(QUrl::fromLocalFile(dir.absoluteFilePath("busytone.wav")));
    audiolist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    player->setPlaylist(audiolist);
    //player->set
}

MainWindow::~MainWindow()
{
    qDebug()<<"deleting MainWindow...";
    delete ui;

//    SipThread * sipthread = Global::instance()->sipthread;
//    if(sipthread)
//        delete sipthread; //有问题，
    //softPhone: ../../../libgomp/oacc-init.c:346: goacc_destroy_thread: Assertion `walk' failed.
    Manager * man = Global::instance()->manager;
    if(man)
        delete man;
//    sipProcess.close();
//    sipthread->quit();
//    sipthread->wait();
    //delete sipthread;
    //sipthread->deleteLater();

    if(player->state() ==  QMediaPlayer::PlayingState)
            player->stop();

//         delete audiolist;
//         delete player;
}

/*
void MainWindow::on_pushButton_1_clicked()
{
    ChatWindow *cw = new ChatWindow(this);
    //cw->setupUi();
    cw->show();
    about();
}

void MainWindow::on_pushButton_2_clicked()
{
    SettingsDialog *setting = new SettingsDialog(this);
    setting->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    VideoDialog *vid = new VideoDialog(this) ;
    vid->show();
*/

//}


void MainWindow::about(){
    QMessageBox ::about(this,tr("关于此软件"),tr("制作人：江川\n13109599891\n2018.8.6"));
}

//void MainWindow::on_pushButton_6_clicked()
//{
//    QMessageBox * m = new QMessageBox(this);
//    m->setText("6");
//    m->show();
//}

void MainWindow::quit(){
    qDebug()<<"quit clicked" ;
    this->close();
}

void MainWindow::showSettingsWin(){
    SettingsDialog  setting ;
    //setting.setAttribute(Qt::WA_DeleteOnClose);
    if(QDialog::Accepted == setting.exec()){
        qDebug()<<"ok clicked";

    }else{
        qDebug()<< "cancel clicked";
    }
}

void MainWindow::on_backspaceButton_clicked()
{
    QLabel * lbl = ui->label_phoneNum;
    QString phoneNum = lbl->text();
    int n = phoneNum.length();
    if(n>0){
        phoneNum = phoneNum.left(n-1);
        lbl->setText(phoneNum);
    }

}
void MainWindow::keyPressEvent(QKeyEvent *event){
    qDebug()<<"keypressed"<<event->key() <<event->text();
    int key = event->key();// 'key' is upper case
    if(event->modifiers() == Qt::ControlModifier){
        if(key == Qt::Key_Z)
            this->showMinimized();
        return;
    }
    if(key >=0x20 && key <=0x7E){
        QLabel * lbl = ui->label_phoneNum;
        QString phoneNum = lbl->text();
        QString text = event->text(); //real text of the key
        phoneNum += text;
        lbl->setText( phoneNum );
    }
    if(key == Qt::Key_Backspace){
        on_backspaceButton_clicked();
    }
    if(key == Qt::Key_Return){
        qDebug()<< "enter pressed";
        call();
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::addNum(QString num){
    QLabel * lbl = ui->label_phoneNum;
    QString phoneNum = lbl->text();
    phoneNum += num;
    lbl->setText(phoneNum);
}

void MainWindow::showVideo(bool show ){
    if (show){
        VideoDialog *vid = new VideoDialog(this) ;
        vid->setAttribute(Qt::WA_DeleteOnClose);
        vid->show();
    }else{
        QList<VideoDialog *> vids = findChildren<VideoDialog*>();
        for (auto vid : vids)
            if(vid){
                qDebug()<<vid;
                vid->close();
                //vid->destroy();
            }
    }

}


//sip
void MainWindow::setupSip(){

}

void MainWindow::startSip(){
//    setupSip();
//    sipProcess.start("./unisip");
}

void MainWindow::call(){
    QLabel * lbl = ui->label_phoneNum;
    QString phoneNum = lbl->text();
    qDebug()<<"call "<< phoneNum;
    emit makecall(phoneNum);
//    QByteArray cmd = QString( "call " + phoneNum+"\n").toLatin1() ;
//    sipProcess.write(cmd);
//    qDebug()<<cmd<<sipProcess.state();

}

void MainWindow::im(){
//    sipProcess.write("im hello from UI.\n");
    qDebug()<<"sending instant msg";
}

void MainWindow::hangup(){
//    QByteArray cmd = "hang\n";
//    sipProcess.write(cmd);

}

void MainWindow::on_pushButton_call_clicked()
{
    call();
}

void MainWindow::on_pushButton_hang_clicked()
{
    //hangup();
    QString number = ui->label_phoneNum->text();
    emit hang(number);
}

void MainWindow::readFromSip(){
//    QByteArray buffer ;
//    buffer = sipProcess.readAllStandardOutput();
//    qDebug()<< buffer;
}

void MainWindow::incommingCall(QString remoteUri){
    //QString id = QString::fromStdString( arg);
    qDebug() << "MainWindow::incommingCall"<< remoteUri;
//    audiolist->setCurrentIndex(RING);
//    audiolist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
//    player->setVolume(100);
//    player->play();
    play(RING,QMediaPlaylist::CurrentItemInLoop);

    QMessageBox msgbox;
    msgbox.addButton("接听",QMessageBox::AcceptRole);
    msgbox.addButton("拒绝",QMessageBox::RejectRole);
    msgbox.setText(remoteUri);
    msgbox.exec();

    if( msgbox.buttonRole(msgbox.clickedButton()) == QMessageBox::AcceptRole){
        QString number;
        int start = remoteUri.lastIndexOf("sip:");
        start += strlen("sip:");
        int end = remoteUri.lastIndexOf("@");
        end -= 1;
        number = remoteUri.mid(start,end-start+1);
        ui->label_phoneNum->setText(number);
        statusLabel.setText("正在与"+number+"通话...");
        emit answer(remoteUri,true);
    }else{
        emit answer(remoteUri,false);
    }
    player->stop();
}

void MainWindow::on_pushButton_msg_clicked()
{
    //im();
    emit  sendmsg("1004","hello , nihao 你好！");
    ChatWindow * chatwin = ChatWindow::instance();
    Global *g  = Global::instance();
    Config * cfg = g->config;
    QString serverip = cfg->server;
    QString uri = "sip:"+ui->label_phoneNum->text()+"@"+serverip;
    chatwin->setCurrentUri(uri);
    chatwin->show();
}

void MainWindow::recvmsg(const OnInstantMessageParam &prm){
    qDebug()<<"MainWindow::recvmsg "  << prm.fromUri.data() << prm.msgBody.data();
    play(NEWMESSAGE,QMediaPlaylist::CurrentItemOnce);
    ChatWindow * chatwin = ChatWindow::instance();
    //chatwin->setAttribute(Qt::WA_DeleteOnClose);
    chatwin->show();
    chatwin->raise();
    chatwin->activateWindow();
}
