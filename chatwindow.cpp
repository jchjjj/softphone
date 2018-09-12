#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QtWidgets>
#include <iostream>
#include <pjsua2.hpp>
#include <sip/Manager.hpp>
#include <sip/head_all.hpp>

using namespace pj;

ChatWindow * ChatWindow::chatwindow = nullptr;
ChatWindow::ChatWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    Manager * manager  = Global::instance()->manager;
    if(manager != nullptr){
        connect(this,SIGNAL(sendMsg(QString,SendInstantMessageParam)),manager,SLOT(sendMsg(QString,SendInstantMessageParam)));
    }

    //readFromFile("message");
    workerThread = new LoadMsgThread("message",message);
    connect(workerThread,SIGNAL(finished()),this,SLOT(msgLoaded()));
    connect(workerThread,SIGNAL(finished()),workerThread,SLOT(deleteLater()));
    workerThread->start();

    connect(ui->sessionlistWidget,SIGNAL(currentRowChanged(int)),this,SLOT(switchPeer(int)));
}

ChatWindow::~ChatWindow()
{
    qDebug()<< "deleting ChatWindow...";
    delete ui;

    writeToFile("message");

    for(auto ptr : message.values()){
        delete ptr;
    }
}

//void ChatWindow::on_pushButton_clicked()
//{
//    QDialog * d = new QDialog(0);
//    QWidget * w = new QWidget(0,Qt::Dialog);
//    w->setWindowTitle(tr("hello new window"));
//    w->resize(100,100);
//    w->show();
//    QLabel l;
//    QLabel *label = new QLabel(d);
//////    label.
//    label->resize(100,20);
//    d->show();
//    std::cout<<"clicked.."<<std::endl;
//}



void ChatWindow::on_closeBtn_clicked()
{
    this->close();
}

void ChatWindow::on_sendBtn_clicked()
{
    SendInstantMessageParam prm;
    prm.content = ui->sendTextEdit->document()->toPlainText().toStdString();
    prm.contentType = "text/plain" ;
    QString uri = ui->urilineEdit->text();
    emit sendMsg(uri,prm);
    ui->sendTextEdit->clear();
    ui->sendTextEdit->setFocus();

    Message m ;
    m.from = "me";
    m.body = QString::fromStdString(prm.content);
    m.time = QDateTime::currentDateTime().toString();
    m.to = ui->urilineEdit->text();
    if(message.contains(m.to)){
        message[m.to]->append(m);
    }else{
        message[m.to] = new QList<Message>;
        message[m.to]->append(m);
    }
    ui->recvTextEdit->append("<b>"+m.from+"</b>"+" ["+m.time+"]:<br>"+m.body);
}

void ChatWindow::recvMsg(const OnInstantMessageParam &prm)
{
    QString msg = QString::fromStdString(prm.fromUri +":\n"+ prm.msgBody);
    qDebug()<<msg;
    QString uri = QString::fromStdString(
                prm.fromUri.substr(1,prm.fromUri.size()-2));

    Message m ;
    m.from = uri;
    m.to = "me";
    m.time = QDateTime::currentDateTime().toString();
    m.body = QString::fromStdString(prm.msgBody);
    if(message.contains(m.from)){
        message[m.from]->append(m);
    }else{
        message[m.from] = new QList<Message>;
        message[m.from]->append(m);
    }
    //ui->urilineEdit->setText(uri);
    setCurrentUri(uri);
    ui->recvTextEdit->append(m.from+" ["+m.time+"]:\n"+m.body);
}

void ChatWindow::readFromFile(const QString & dirname){
    QDir dir(dirname);
    if(!dir.exists()){
        qDebug()<< "要读取的目录不存在";
        return;
    }
    dir.setFilter(QDir::Files);
    for(QString & filename : dir.entryList()){
        qDebug()<<"ChatWindow::readFromFile :"<<filename;
        QFile file(dir.filePath(filename));
        if(! file.open(QIODevice::ReadOnly) ){
            QMessageBox::information(this,tr("文件无法打开！"), file.errorString());
            return;
        }

        QList<Message>  * msglist = new QList<Message>;
        QDataStream in( &file);
        in >> *msglist;
        if(msglist->isEmpty()){
            QMessageBox::information(this,tr("没有已保存的信息"),tr("文件为空，或读取失败"));
        }else{
            auto key = (*msglist)[0].from;
            message[key] = msglist;
        }
    }

}

void ChatWindow::writeToFile(const QString & dirname){
    QDir dir;
    qDebug()<<"dir path : "<<dir.absolutePath();
    if(!dir.exists(dirname)){
        bool success = dir.mkpath(dirname);
        if(!success){
            qDebug()<<"创建目录失败";
            return;
        }
        dir.cd(dirname);

    }else{
        dir.cd(dirname);
    }
    for(QString & key : message.keys() ){
        qDebug()<< dir.filePath(key);
        QFile file(dir.filePath( key) );
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("文件无法打开！"), file.errorString());
            return;
        }
        QDataStream out(&file);
        out << *message[key];

        file.close();
    }
}

//void ChatWindow:: loadinBackground(const QString &dirname, QMap<QString,  QList<Message>* > &message){
//    qDebug()<<"ChatWindow:: loadinBackground :" << QThread::currentThread();
//    readFromFile(dirname);
//}
void ChatWindow:: msgLoaded(){
    qDebug()<< "msg loaded" << message;

    for(QString &key : message.keys()){
        ui->sessionlistWidget->addItem(key);
    }
//    workerThread->quit();
//    workerThread->wait();
}

QString & ChatWindow:: formatMsg(QString & uri){
    static QString nomsg = "空";
    static QString data;
    QTextStream qts(&data);
    data.clear();
    if(message.contains(uri)){
        auto list = message[uri];
        for (Message msg : *list){
            qts<< msg.from << " [" << msg.time<<"]:\n" << msg.body << "\n";
        }
        return *qts.string();
    }else{
        return nomsg;
    }

}

void ChatWindow::switchPeer(int index){
    QString uri;
    auto item = ui->sessionlistWidget->item(index);
    uri = item->text();
    setCurrentUri(uri);

    //ui->recvTextEdit->scroll(-1,-1);
}
void ChatWindow::setCurrentUri(QString uri){
    currentUri = uri;
    ui->urilineEdit->setText(uri);
    //int index = -1;
    auto list = ui->sessionlistWidget->findItems(uri,Qt::MatchExactly);
    if(list.size() == 1){
        if(list[0] != ui->sessionlistWidget->currentItem())
            ui->sessionlistWidget->setCurrentItem(list[0],QItemSelectionModel::SelectCurrent);
    }else if(list.size() == 0){
        ui->sessionlistWidget->addItem(uri);
        int count = ui->sessionlistWidget->count();
        ui->sessionlistWidget->setCurrentItem(
                    ui->sessionlistWidget->item(count-1),QItemSelectionModel::SelectCurrent);
    }else{
        qDebug()<<"error :void setCurrentUri(QString uri) ";
    }
    ui->recvTextEdit->setText(formatMsg(uri));
    QScrollBar *scrollbar = ui->recvTextEdit->verticalScrollBar();
    scrollbar->setSliderPosition(scrollbar->maximum());
}
