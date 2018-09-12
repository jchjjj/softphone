#ifndef SIPTHREAD_H
#define SIPTHREAD_H

#include <QObject>
#include <QThread>
#include "sip/Manager.hpp"
class Manager;

class SipThread : public QThread
{
    Q_OBJECT
public:
    SipThread();
    SipThread(QObject* parent);
    ~SipThread();
    Manager * sipman;
signals:
    void incommingCall(QString id);

    void msgReceived(QString id , QString text);
protected:
    void run() override;

public slots:
    void answer(QString arg);
    void call(QString);
    void hangup(QString id);
    void sendmsg(QString id , QString text);
};

#endif // SIPTHREAD_H
