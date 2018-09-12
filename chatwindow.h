#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDialog>
#include "ui_chatwindow.h"
#include <pjsua2.hpp>
#include <model/message.h>
#include <QDir>
#include <QtWidgets>

using namespace pj;

namespace Ui {
class ChatWindow;
}
class LoadMsgThread;

class ChatWindow : public QDialog
{
    Q_OBJECT

public:
    static ChatWindow * instance(){
        if (chatwindow == nullptr){
            chatwindow = new ChatWindow;
        }
        return chatwindow;
    }
    static ChatWindow * instance(QWidget * parent){
        if (chatwindow == nullptr){
            chatwindow = new ChatWindow(parent);
        }
        return chatwindow;
    }
    ~ChatWindow();
signals:
    void sendMsg(QString uri,SendInstantMessageParam prm);

private slots:

    void on_closeBtn_clicked();

    void on_sendBtn_clicked();

    void recvMsg(const OnInstantMessageParam &prm);
    void msgLoaded();
    void switchPeer(int index);

public:
    void setCurrentUri(QString uri);

private:
    Ui::ChatWindow *ui;
    static ChatWindow * chatwindow;
    explicit ChatWindow(QWidget *parent = 0);

    //worker thread
    LoadMsgThread * workerThread;

    QString currentUri;

    //typedef QList<Message>* MessageList;
    QMap<QString,  QList<Message>* > message;


    void readFromFile(const QString & dirname);
    void writeToFile(const QString & dirname);
    QString &formatMsg(QString & uri);

    //void loadinBackground(const QString &dirname, QMap<QString,  QList<Message>* > &message);

};

class LoadMsgThread:public QThread
{
    Q_OBJECT
    public :
    LoadMsgThread(const QString dir,QMap<QString,  QList<Message>* > &msg):QThread(),
    dirname(dir),message(msg){
    }
private:
    const QString  dirname;
    QMap<QString,  QList<Message>* > &message;

protected:
    void run(){
        qDebug()<<dirname;
        QDir dir(dirname);
        if(!dir.exists()){
            qDebug()<< "要读取的目录不存在";
            return;
        }
        dir.setFilter(QDir::Files);
        for(QString & filename : dir.entryList()){
            qDebug()<<"LoadMsgThread::readFromFile :"<<filename;
            QFile file(dir.filePath(filename));
            if(! file.open(QIODevice::ReadOnly) ){
                QMessageBox::information(NULL,tr("文件无法打开！"), file.errorString());
                return;
            }

            QList<Message>  * msglist = new QList<Message>;
            QDataStream in( &file);
            in >> *msglist;
            if(msglist->isEmpty()){
                QMessageBox::information(NULL,tr("没有已保存的信息"),tr("文件为空，或读取失败"));
            }else{
                QString key = (*msglist)[0].from;
                if(key == "me")
                    key = (*msglist)[0].to;
                message[key] = msglist;
            }
        }
        //exec(); //没有exec()时，可以发出信号，但不能接收和处理信号
    }
};

#endif // CHATWINDOW_H
