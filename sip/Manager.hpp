#pragma once
//#include "stdafx.h"

#include <QObject>

#include <pjsua2.hpp>
#include <iostream>
#include <memory>
#include <pj/file_access.h>
#include "head_all.hpp"
#include  <stdint.h>

#include "sipthread.h"


using namespace pj;
using namespace std;

enum PORTTYPE
{
    UDP = 0,
    TCP = 1

};


class Manager:public QObject
{
    Q_OBJECT
public :
    vector<sipAccount *> accounts;
    //EpConfig epConfig;
    UaConfig uaConfig;
    MediaConfig mediaConfig;
    LogConfig logConfig;

    Endpoint ep; //ONLY one ep in all scope!!
    EpConfig ep_cfg;
    TransportConfig tcfg;
    AccountConfig acc_cfg;
    sipAccount * currentAccount;
    std::auto_ptr<sipAccount> acc;
    Call *call;
    bool callisActive = false;

    //sip server IP
    string sipServer ;//= "192.168.199.227";

private:
    uint16_t port;
    PORTTYPE portType;

    QObject * sipthread;
    QObject *s;

public:
    Manager();
    Manager(QObject *thread);
    ~Manager();
    void init();
    void loadCfg();
    void Start();
    // DWORD WINAPI StartListen(PVOID arg);
    int StopListen(void *);
    int StartEndPoint();
    int AddAccount(AccountConfig cfg);
    int MakeCall(sipAccount &acc, Buddy & buddy, CallOpParam &prm);
    int MakeCall(sipAccount &acc, string uri, CallOpParam &prm);
    int MakeCall(string uri);
    int StopSipThread(void*);
    int RestartSipThread(void*);
    void ProcessCMD(char * data);

    int AddBuddy(BuddyConfig &cfg);
private:
    void registerThread(string name);

private slots:
    void sendmsg(QString id , QString text);
    void sendMsg(QString uri,SendInstantMessageParam prm);
};

