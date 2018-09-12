//#include "stdafx.h"
#include "Manager.hpp"
#include <string>
#include <sstream>
#include <iostream> 
#include <unistd.h>
#include <QDebug>
#include <model/config.h>

Manager::Manager()
{
    qDebug() <<"Manager::Manager() :" <<  QThread::currentThread();
}

Manager::Manager(QObject *thread){
    this->sipthread = thread;
}

Manager::~Manager()
{
    qDebug()<< "deleting Manager....";
    try{
        qDebug()<< "Manager::~Manager() : thread: "<<QThread::currentThread()
                   << ep.libIsThreadRegistered();
        registerThread("Manager");
        ep.libDestroy(0);
    }catch(Error & e){
        qDebug() <<e.title.data()<< e.reason.data()<<endl;
    }


}


//load config from file
void Manager:: loadCfg() {
    //loadCfg();
    JsonDocument jdoc;
    try{
        jdoc.loadFile("account.json");
        jdoc.readObject(acc_cfg);
    }catch(Error &e){
        qDebug()<<e.info().data()<<endl;
    }
    Config * cfg = Global::instance()->config;
    this->sipServer = cfg->server.toStdString();

}

//init config
void  Manager::init() {
    //null members
    currentAccount = nullptr;
    call = nullptr;

    loadCfg();


	// mannual config
//	acc_cfg.idUri = "sip:1000@"+sipServer;
//	acc_cfg.regConfig.registrarUri = "sip:"+sipServer;
//	acc_cfg.sipConfig.authCreds.push_back(AuthCredInfo("digest", "*",
//        "1000", 0, "1000"));
//    acc_cfg.callConfig.timerMinSESec = 90; //
//	acc_cfg.callConfig.timerSessExpiresSec = 1200;// must NOT lowwer than timerMinSESec!!!

//    registerThread("manager");
//    JsonDocument jdoc;
//    jdoc.writeObject(acc_cfg);
//    jdoc.saveFile("account.json");
}
void Manager::Start() {
	cout << "starting EndPoint" << endl;
	StartEndPoint();

	cout << "Add SipAccount" << endl;
	AddAccount(acc_cfg);
	currentAccount = accounts[0];

	cout << "Add Buddy" << endl;
	BuddyConfig cfg[3];
	for (int i = 0; i < 3; i++) {
		/*string uri;
		uri.clear();*/
		ostringstream oss;
		oss << "sip:" << 1004+ i << "@" << sipServer;
		cout << oss.str() << endl;
		cfg[i].uri = oss.str();
		cfg[i].subscribe = true;
		AddBuddy(cfg[i]);
	}

    registerThread("Manager");
	cout << "Manager started." << endl;
	//HANDLE threadSip = CreateThread(NULL, 0, this->StartListen, NULL, 0, NULL);
	//_beginthreadex(NULL, 0, StartCMDServer, this, 0, NULL);
    //pj_thread_register("Manager");

}
void Manager::registerThread(string name){
    if(!ep.libIsThreadRegistered()){
         ep.libRegisterThread(name);
         qDebug() << QThread::currentThread()<<" thread registered..";
     }
}

vector< string> split(string str, string pattern)
{
	vector<string> ret;
	if (pattern.empty()) return ret;
	size_t start = 0, index = str.find_first_of(pattern, 0);
	while (index != str.npos)
	{
		if (start != index)
			ret.push_back(str.substr(start, index - start));
		start = index + 1;
		index = str.find_first_of(pattern, start);
	}
	if (!str.substr(start).empty())
		ret.push_back(str.substr(start));
	return ret;
}

#define isCMD(cmd) !strncmp(cmd,data,strlen(cmd))
void Manager::ProcessCMD(char * data) {
	if (!strncmp("call", data, 4)) {
		CallOpParam prm(true);
		prm.opt.audioCount = 1;
		prm.opt.videoCount = 0;
		BuddyVector bv =  currentAccount->enumBuddies();
		string cmd(data);
		vector<string> args = split(cmd, " ");
		if (args.size() >= 2)
		{
			Buddy * buddy = NULL;
			string phonecode = args[1];
			Buddy * bdy = NULL;
            for (size_t i=0;i<bv.size();i++){
				bdy = bv[i];
				BuddyInfo bi = bdy->getInfo();
                if (bi.uri.find(phonecode) != std::string::npos) {
					buddy = bdy;
					break;
				}	
			}

			if(buddy != NULL)
				this->MakeCall(*(currentAccount),*buddy, prm);
		}
		else {
			cout << "######### no phone number!!! Use default num:1001." << endl;
			this->MakeCall(*currentAccount, "sip:1004@" + sipServer, prm);
		}
		
	}
	else if (isCMD("im")) { //instant msg
		//currentAccount->calls;
		pj::Call * call ;
        std::vector<pj::Call *> calls = currentAccount->calls;
		int c = calls.size();
		for (int i = 0 ; i<c ; i++	) {
			call = calls[i];
			//call->hangup(cop);
			if (call != NULL) { //send im from a active call
				CallInfo info = call->getInfo();
				if (info.state == PJSIP_INV_STATE_CONFIRMED) {
					SendInstantMessageParam prm;
					prm.content = (char * ) (data+3);

					call->sendInstantMessage(prm);
				}
				cout << "sending im to : " << info.remoteUri<< endl;
			}
		}
		if(c == 0){ //send from a buddy
			cout << "#####sending im to all buddies....." << endl;
			BuddyVector bv = currentAccount->enumBuddies();
			if (bv.size() > 0) {
				Buddy*  buddy ;
				for (BuddyVector::iterator it = bv.begin();it!=bv.end();it++)
				{
					buddy = *it;
					SendInstantMessageParam prm;
					prm.content = (char * ) (data+3);
					prm.content = "Buddy: " + prm.content;
					buddy->sendInstantMessage(prm);
				}	
			}
		}
		/*
		if (call != NULL) { //send im from a active call
			CallInfo info = call->getInfo();
			if (info.state == PJSIP_INV_STATE_CONFIRMED) {
				SendInstantMessageParam prm;
				prm.content = "hello ,I am from pjsua2";

				call->sendInstantMessage(prm);
			}
			cout << "sending im to : " << info.remoteUri<< endl;
		}
		else {

		} */
	}
	else if (isCMD("play")) { //play music
		AudioMediaPlayer * player = new AudioMediaPlayer(); //�������ջ�ϣ��ᱻ���٣����ܵ�������Ų���ȫ��
		AudioMedia& play_med = Endpoint::instance().audDevManager().getPlaybackDevMedia();
		try {
			char path[256];
			pj_bzero(path, 256);
			getcwd(path, 255);

			/*char chpath[MAX_PATH];
			GetModuleFileName(NULL, chpath, MAX_PATH);
			string filename =  chpath;*/
			char * filepath = strcat(path, "input.44.wav");
			player->createPlayer(filepath, 
				PJMEDIA_FILE_NO_LOOP);
			player->startTransmit(play_med);
			//Sleep(4000);
		}
		catch (Error& err) {

		}

	}
	else if (isCMD("pres")) { //presence

	}
	else if (isCMD("hang")) { // hangup all calls
		CallOpParam cop;
		cop.reason = "hang";
		cop.statusCode = PJSIP_SC_OK;
		pj::Call * call ;
		std::vector<pj::Call *> calls = currentAccount->calls;
		int c = calls.size();
		for (int i = 0 ; i<c ; i++	) {
			call = calls[i];
			call->hangup(cop);
		}
		currentAccount->calls.clear();

	}
	else{
		cout<<"\nUNKNOWN CMD.！！！"<<endl;
	}	

	//delete data;
}

int Manager::StartEndPoint() {
	int ret = 0;

	try {
		ep.libCreate();
		ret = PJ_SUCCESS;
	}
	catch (Error & err) {
		std::cout << "Exception: " << err.info() << std::endl;
		ret = 1;
		return ret;
	}


	try {
        //load from file
        JsonDocument jdoc;
        jdoc.loadFile("endpoint.json");
        jdoc.readObject(ep_cfg);
		// Specify customization of settings in ep_cfg
//		ep_cfg.logConfig.level = 5;
//        ep_cfg.uaConfig.maxCalls = 10;
//		ep_cfg.medConfig.sndClockRate = 16000;
		ep.libInit(ep_cfg);
	}
	catch (Error& err) {
		cout << "Initialization error: " << err.info() << endl;
	}


	// Transport
	try {
        JsonDocument jdoc;
        try{
            jdoc.loadFile("transport.json");
            jdoc.readObject(tcfg);
        }catch(Error & e){
            cout<< e.info()<<endl;
        }

//        tcfg.port = 6060; //default 5060
        TransportId tid = ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
        std::cout << "TransportId: "<< tid <<std::endl;
//        jdoc.writeObject(tcfg);
//        jdoc.saveFile("transport.json");
	}
	catch (Error& err) {
		cout << "Transport creation error: " << err.info() << endl;
	}

	// Start library
	ep.libStart();
	std::cout << "*** PJSUA2 STARTED ***" << std::endl;

    //save epconfig
//    JsonDocument jDoc ;
//    jDoc.writeObject(ep_cfg);
//    jDoc.saveFile("endpoint.json");

	return ret;
}

int Manager::AddAccount(AccountConfig cfg) {
	sipAccount * acc = new sipAccount();
	try
	{
		acc->create(cfg);
		//acc->setDefault();
		//currentAccount = acc; // 2018
	}
	catch (Error& err) {
		cout << "Account creation error: " << err.info() << endl;
		return -1;
	}
	
	accounts.push_back(acc);
	return 0;
}

int Manager::AddBuddy(BuddyConfig &cfg) {
	sipBuddy * buddy = new sipBuddy();
	if (currentAccount != NULL) {
		buddy->create(*currentAccount, cfg);
	}
	return 0;
}

int Manager::MakeCall(sipAccount & acc, Buddy & buddy,CallOpParam & prm) {
	call = new sipCall(acc);
	acc.calls.push_back(call);
	string uri;
	BuddyInfo bi = buddy.getInfo();
	uri = bi.uri;
	call->makeCall(uri, prm);
	return 0;
}

int Manager::MakeCall(sipAccount &acc, string uri, CallOpParam &prm) {
	call = new sipCall(acc);
	acc.calls.push_back(call);
	call->makeCall(uri, prm);
	return 0;
}

int Manager::MakeCall(string uri){
    registerThread("Manager");
    qDebug()<< "int Manager::MakeCall(string uri)"<< QThread::currentThread();
    CallOpParam prm(true);
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 0;
    call = new sipCall(*currentAccount);
    currentAccount->calls.push_back(call);
    if(uri.find("@") == std::string::npos){
        uri = "sip:"+ uri +"@" + this->sipServer;
    }
    call->makeCall(uri,prm);
    return 0;
}

void Manager::sendmsg(QString id , QString text){
    registerThread("Manager");
    pj::Call * call ;
    std::vector<pj::Call *> calls = currentAccount->calls;
    int c = calls.size();
    for (int i = 0 ; i<c ; i++	) {
        call = calls[i];
        //call->hangup(cop);
        if (call != NULL) { //send im from a active call
            CallInfo info = call->getInfo();
            if (info.state == PJSIP_INV_STATE_CONFIRMED) {
                SendInstantMessageParam prm;
                prm.content = text.toStdString();

                call->sendInstantMessage(prm);
            }
            cout << "sending im to : " << info.remoteUri<< endl;
        }
    }
    if(c == 0){ //send from a buddy
        cout << "#####sending im to all buddies....." << endl;
        BuddyVector bv = currentAccount->enumBuddies();
        if (bv.size() > 0) {
            Buddy*  buddy ;
            for (BuddyVector::iterator it = bv.begin();it!=bv.end();it++)
            {
                buddy = *it;
                SendInstantMessageParam prm;
                prm.content = text.toStdString();
                prm.content = "Buddy: " + prm.content;
                buddy->sendInstantMessage(prm);
            }
        }
    }
}// end of Manager::sendmsg

void Manager::sendMsg(QString uri,SendInstantMessageParam prm){
    registerThread("Manager");
    //uri+=QString::fromStdString(this->sipServer);// not need .....
    bool found = false;

    for (Buddy * bdy :currentAccount->enumBuddies()){
        if(bdy->getInfo().uri == uri.toStdString() ){
            bdy->sendInstantMessage(prm);
            found = true;
        }
    }
    if(!found){
        BuddyConfig bcfg;
        bcfg.uri = uri.toStdString();
        bcfg.subscribe = true;
        AddBuddy(bcfg);
        Buddy * bdy = currentAccount->findBuddy(uri.toStdString());
        if(bdy != NULL){
            bdy->sendInstantMessage(prm);
        }
    }
}
