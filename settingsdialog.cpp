#include "settingsdialog.h"
#include "ui_settingsdialog.h"
//#include "ui_settingsdialog.h"
#include "ui_videodialog.h"
#include "videodialog.h"

#include <QDialog>
#include <QtWidgets>
#include <iostream>
#include <QDebug>
#include <global.h>
#include <model/config.h>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    //std::cout<<"#include settingsdialog.h"<<std::endl;
    qDebug("SettingsDialog constructor");
    //connect(this,SIGNAL(),);

    //init args
    Config * cfg = Global::instance()->config;
    if(cfg != nullptr){
        ui->usernameLineEdit->setText(cfg->username);
        ui->passwdLineEdit->setText(cfg->passwd);
        ui->serverLineEdit->setText(cfg->server);
        int size = cfg->serverIPs.size();
        if(size > 2) size =2;
        switch(size){
        case 2:
            ui->sipServerIP2->setText(cfg->serverIPs[1]);
        case 1:
            ui->sipServerIP1->setText(cfg->serverIPs[0]);
        case 0:
            ;
        default:
            ;
        }
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

//void SettingsDialog::on_pushButton_clicked()
//{
//    QMessageBox *msgbox = new QMessageBox(this);
//    msgbox->setText("nihao mahhll h?");
//    msgbox->resize(200,100);
////    msgbox->setTextFormat();
//    msgbox->show();

//}

//void SettingsDialog::on_pushButton_2_clicked()
//{
//    //right version
//    VideoDialog * video = new VideoDialog(this);
//    video->show();
//}

//void SettingsDialog::on_pushButton_3_pressed()
//{
//    QDialog *dialog = new QDialog(this);
//    Ui::VideoDialog vid ;
//    vid.setupUi(dialog);
//    dialog->setWindowTitle("only ui, no actions!");
//    dialog->show();
//}

void SettingsDialog::on_buttonBox_accepted()
{
    qDebug()<<"SettingsDialog::on_buttonBox_accepted" << ui->usernameLineEdit->text();
    Config * cfg = Global::instance()->config;
    if(cfg != nullptr){
        cfg->username = ui->usernameLineEdit->text();
        cfg->passwd   = ui->passwdLineEdit->text();
        cfg->server   = ui->serverLineEdit->text();
        cfg->serverIPs.clear();
        cfg->serverIPs.append(ui->sipServerIP1->text());
        cfg->serverIPs.append(ui->sipServerIP2->text());
        cfg->save();
    }
    accept();
}

void SettingsDialog::on_buttonBox_rejected()
{
    reject();
}
