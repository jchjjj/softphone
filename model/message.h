#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

struct Message
{
public:
    Message();

    QString from;
    QString to;
    QString time;
    QString body;
};

inline QDataStream &operator<<(QDataStream &stream, const Message &msg)
{
    return stream << msg.from << msg.to<< msg.time << msg.body ;
}

inline QDataStream &operator>>(QDataStream &stream, Message &msg)
{
    return stream >> msg.from >> msg.to >> msg.time >> msg.body ;
}
#endif // MESSAGE_H
