#include "sipthread.h"
#include <QThread>
#include <QDebug>
#include "sip/Manager.hpp"

SipThread::SipThread()
{
    sipman = new Manager();
}
SipThread::SipThread(QObject* parent):QThread(parent){
    sipman = new Manager();
}

SipThread::~SipThread(){
    qDebug()<<"SipThread destroying...";
    qDebug()<<"deleting sipman...";

    delete sipman;
    quit();
    //terminate();
    //usleep(1000);
    //requestInterruption();

    wait();
}
void SipThread::run(){
    qDebug() <<"void SipThread::run():" <<  QThread::currentThread();
    sipman->ep.libRegisterThread("sipThread");
    qDebug()<< sipman->ep.libIsThreadRegistered();
    sipman->init();
    sipman->Start();


    exec();
//    while (!isInterruptionRequested()) {
//        qDebug()<<"sip thread ...";
//        sleep(1);
//    }
}

void SipThread::answer(QString arg){
    qDebug()<<"answer call " << arg;
}

void SipThread::call(QString uri){
    string uri2 = uri.toStdString();
    sipman->MakeCall(uri2);
}
void SipThread::hangup(QString id){
    qDebug()<<"sip thread"<<id;
//    sipman->hangup(id.toStdString());
}

void SipThread::sendmsg(QString id , QString text){
    qDebug()<< "SipThread::sendmessage : " << id << text;
}
