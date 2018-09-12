#include "numberkey.h"
#include <QtWidgets>
#include <util.h>

NumberKey::NumberKey()
{

}
NumberKey::NumberKey(QWidget * parent):QPushButton(parent){

}

void NumberKey::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton){
        QString num = this->text();
        emit numPressed(num);
    }
}

void NumberKey::stateChange(CallState s){
    this->callstate = s;
}
