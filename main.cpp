#include "mainwindow.h"
#include <QApplication>
#include <database.h>
#include <global.h>
#include <sipthread.h>
#include <QDebug>
#include <chatwindow.h>
#include <model/config.h>

void init(){
    qRegisterMetaType<OnInstantMessageParam>("OnInstantMessageParam");//先要向qt注册这个类型，才能在connect中使用
    qRegisterMetaType<SendInstantMessageParam>("SendInstantMessageParam");
}

void loadCfg(){
    Global *g  = Global::instance();
    Config * cfg = g->config;
    bool res = cfg->load();
    if(!res){
        QMessageBox::critical(nullptr,"配置文件加载失败","请检查当前目录下有无config.json配置文件");
        exit(-1);
    }
//    cfg->username = "1001";
//    cfg->passwd = "0000";
//    cfg->server = "192.168.199.227";
//    cfg->serverIPs.append("192.168.199.227");
//    cfg->serverIPs.append("192.168.0.2");
//    cfg->save();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //db init
    DataBase db("./data.db");
    db.init();
    //global init
    init();
    //
    Global *g  = Global::instance();

    //loadCfg
    loadCfg();

    MainWindow w;
    g->mainwindow = &w;  // mainwindow must be created before sipthread
    qDebug() << g->mainwindow;

    //
    g->sipthread = new SipThread();
    g->manager = g->sipthread->sipman;
    //start sip thread
    //sipthread = new SipThread();
    SipThread *sipthread = (g->sipthread);
    sipthread->sipman->ep.libRegisterThread("mainthread");
    qDebug()<<sipthread->sipman->ep.libIsThreadRegistered();

    qDebug()<<"main thread: "<<  w.thread()<<"sipthread:"<< sipthread->thread()<<
              "sipman: "<<sipthread->sipman->thread();
    qDebug() <<"in main function :" <<  QThread::currentThread();
    QObject::connect(&w,SIGNAL(makecall(QString)),sipthread,SLOT(call(QString)),Qt::QueuedConnection);
    QObject::connect(&w,SIGNAL(sendmsg(QString,QString)),sipthread,SLOT(sendmsg(QString,QString)),Qt::QueuedConnection);
    QObject::connect(&w,SIGNAL(sendmsg(QString,QString)),sipthread->sipman,SLOT(sendmsg(QString,QString)),Qt::QueuedConnection);


    //init chatwindow
    g->chatwindow = ChatWindow::instance(&w);


    sipthread->start();

    w.show();

    return a.exec();
}
