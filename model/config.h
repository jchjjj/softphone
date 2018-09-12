#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <pjsua2.hpp>
#include <QJsonObject>
#include <QString>
#include <QVector>

class Config
{
public:
    Config();


    QString username;
    QString passwd;
    QString server;

    QVector< QString > serverIPs;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool save();
    bool load();
};

#endif // CONFIG_H
