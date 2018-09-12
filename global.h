#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>

class MainWindow;
class Manager;
class ChatWindow;
class SipThread;
class Config;

class Global
{
public:

    static Global * instance(){
        if(global == NULL){
            global = new Global;
        }
        return global;
    }

    MainWindow * mainwindow;
    Manager * manager;
    ChatWindow * chatwindow;
    SipThread * sipthread;
    Config * config;

private:
    Global();
    static Global * global;
};

#endif // GLOBAL_H
