//#include "stdafx.h"
#include "sipAccount.hpp"
#include <iostream>
#include "head_all.hpp"
#include <QObject>
#include <QString>
#include <global.h>
#include <mainwindow.h>
#include <QDebug>
#include <QMetaType>
#include <chatwindow.h>

using namespace std;


sipAccount::sipAccount():Account()
{
    std::cout <<"sipAccount::sipAccount()"<< " constructor";
    MainWindow * mainwindow ;
    Global * global = Global::instance();
    if(global->mainwindow != NULL){
        mainwindow = global->mainwindow;
        //connect
        QObject::connect(this,SIGNAL(incommingcall(QString)),mainwindow,SLOT(incommingCall(QString)));
        QObject::connect(mainwindow,SIGNAL(answer(QString,bool)),this,SLOT(answer(QString,bool)));
        connect(mainwindow,SIGNAL(hang(QString)),this,SLOT(hang(QString)));
        QObject::connect(this,SIGNAL(recvmsg(OnInstantMessageParam)),mainwindow,SLOT(recvmsg(OnInstantMessageParam)));
    }else{
        qDebug()<<"sipAccount::sipAccount() : "<<"can't find mainwindow in Global...";
    }
    ChatWindow * chatwin = ChatWindow::instance();
    if(chatwin != nullptr){
        connect(this,SIGNAL(recvmsg(OnInstantMessageParam)),chatwin,SLOT(recvMsg(OnInstantMessageParam)));
    }
}


sipAccount::~sipAccount()
{
    std::cout << "*** Account is being deleted: No of calls="
              << calls.size() <<"SHOULD DELETE ALL CALLS!!!"<< std::endl;
}

void sipAccount::answer(QString remoteUri, bool yesorno){

    for(auto call : calls){
        CallInfo ci = call->getInfo();
        if(ci.remoteUri == remoteUri.toStdString()){
            CallOpParam prm ;
            if(yesorno){ // answer
                prm.statusCode = (pjsip_status_code)200;
                call->answer(prm);
                qDebug()<<"answer";
                break;
            }
            else{ //hang
                prm.statusCode = (pjsip_status_code)PJSIP_SC_BUSY_HERE ;// 486,
                call->hangup(prm);
                qDebug()<<"hang";
                break;
            }
        }
    }
}

void sipAccount::hang(QString number){
    for(auto call:calls){
        CallInfo ci = call->getInfo();
        if(string::npos != ci.remoteUri.find(number.toStdString())){
            CallOpParam prm;
            prm.statusCode = (pjsip_status_code)PJSIP_SC_BUSY_HERE ;// 486,
            call->hangup(prm);
        }

    }
}

void sipAccount::onRegState(OnRegStateParam &prm)
{
    AccountInfo ai = getInfo();
    std::cout << (ai.regIsActive ? "*** Register: code=" : "*** Unregister: code=")
              << prm.code << std::endl;
}

void sipAccount::onIncomingCall(OnIncomingCallParam &iprm)
{
    //Call *call = new sipCall(*this, iprm.callId);
    Call *call = new sipCall(*this, iprm.callId);
    CallInfo ci = call->getInfo();
    CallOpParam prm;

    std::cout << "*** Incoming Call: " << ci.remoteUri << " ["
              << ci.stateText << "]" << std::endl;
    calls.push_back(call);

    emit incommingcall(QString::fromStdString(ci.remoteUri));
    //	prm.statusCode = (pjsip_status_code)200;

    //    call->answer(prm);


}

void sipAccount::onInstantMessage(OnInstantMessageParam &prm) {
    cout << "[sipAccount::onInstantMessage]  :"<< prm.fromUri << " send: " << prm.msgBody << endl;
    emit recvmsg(prm);
}

void sipAccount::onInstantMessageStatus(OnInstantMessageStatusParam &prm) {
    cout << "[sipAccount onInstantMessageStatus]  to: " << prm.toUri << prm.msgBody << endl;
}

void  sipAccount::onIncomingSubscribe(OnIncomingSubscribeParam &prm) {
    cout << "[sipAccount::onIncomingSubscribe]" << prm.fromUri << "suscribe to you." <<
            prm.reason << endl;

}
