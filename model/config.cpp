#include "config.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

Config::Config()
{

}

void Config::read(const QJsonObject &json){
    if (json.contains("username") && json["username"].isString())
        username = json["username"].toString();
    if (json.contains("passwd") && json["passwd"].isString())
        passwd = json["passwd"].toString();
    if (json.contains("server") && json["server"].isString())
        server = json["server"].toString();

    if (json.contains("serverIPs") && json["serverIPs"].isArray()) {
        QJsonArray ipArray = json["serverIPs"].toArray();
        serverIPs.clear();
        serverIPs.reserve(ipArray.size());
        for (int index = 0; index < ipArray.size(); ++index) {
            auto ip = ipArray[index].toString();

            serverIPs.append(ip);
        }
    }
}

void Config::write(QJsonObject &json) const{
    json["username"] = username;
    json["passwd"] = passwd;
    json["server"]  =   server;
    QJsonArray IPs;
    foreach (const QString ip, serverIPs) {
        //QJsonObject ipObject;
        //ip.write(ipObject);
        QJsonValue ipval(ip);
        IPs.append(ipval);
    }
    json["serverIPs"] = IPs;
}

bool Config::save(){
    QFile saveFile(QStringLiteral("config.json"));

          if (!saveFile.open(QIODevice::WriteOnly)) {
              qWarning("Couldn't open save file.");
              return false;
          }

          QJsonObject saveObject;
          write(saveObject);
          QJsonDocument saveDoc(saveObject);
          saveFile.write(saveDoc.toJson());

          return true;
}

bool Config::load(){
    QFile file(QStringLiteral("config.json"));
    if(!file.open(QIODevice::ReadOnly)){
        qWarning("Couldn't open config.json file.");
        return false;
    }
    QByteArray buffer = file.readAll();
    QJsonDocument jdoc(QJsonDocument::fromJson(buffer));
    read(jdoc.object());

    qDebug()<< username << passwd << server << serverIPs;
    return true;
}
