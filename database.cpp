#include "database.h"
#include <QSqlDatabase>
#include <QDebug>
#include<QSqlQuery>
#include <QSqlError>

DataBase::DataBase()
{

}

DataBase::DataBase(QString db):dbname(db)
{

}

DataBase::~DataBase()
{
    db.close();
}

bool DataBase::init(){
    //创建连接
    db = QSqlDatabase::addDatabase( "QSQLITE" );//第二个参数可以设置连接名字，这里为default

    db.setDatabaseName( dbname );// 设置数据库名与路径,
    //打开连接
    if( !db.open() )
    {
        qDebug() << db.lastError() ;
        qFatal( "Failed to connect." );
    }

    qDebug()<< "Connected!" << dbname;
    //各种操作
    QSqlQuery qry;
    //创建table
    bool ok = qry.prepare( "CREATE TABLE IF NOT EXISTS accounts (id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT,  name VARCHAR(30), phoneNum VARCHAR(20),passwd VARCHAR(30),server Varchar(60))" );
    if(!ok){
        qDebug()<< "qry prepare error...";
    }
    if( !qry.exec() ){

        qDebug() << qry.lastError()<<"accounts";
        return false;
    }
    else
        qDebug() << "Table accounts created!";

    ok = qry.exec("insert into accounts(name,phoneNum,passwd,server) values('江','1001','1234','192.168.0.103')");
    if (!ok)
        qDebug()<<"insert error "<<qry.lastError();
    ok = qry.exec("insert into accounts(name,phoneNum,passwd,server) values('川','1002','1234','192.168.0.103')");
    if (!ok)
        qDebug()<<"insert error "<<qry.lastError();
    ok = qry.prepare("create table if not exists buddies (id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT ,name varchar(30),url varchar(45)  )");

    if( !qry.exec() ){
        qDebug() << qry.lastError()<<"buddies";
        return false;
    }
    else
        qDebug() << "Table buddies created!";

    return true;

}
