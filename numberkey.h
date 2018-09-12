#ifndef NUMBERKEY_H
#define NUMBERKEY_H

#include <QWidget>
#include <QtWidgets>
#include <util.h>

//enum class CallState;

class NumberKey : public QPushButton
{
    Q_OBJECT
public:
    NumberKey();
    explicit NumberKey(QWidget * parent);
//enum state{
//    IDLE,
//    CALLING
//};

signals:
    void numPressed(QString num);

private slots:
    void stateChange(CallState s);
protected:
    void mousePressEvent(QMouseEvent * event) override;
private:
    CallState callstate;
};

#endif // NUMBERKEY_H
