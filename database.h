#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class DataBase
{
public:
    DataBase();
    DataBase(QString db);
    ~DataBase();

    bool init();

    private:
    QString dbname;
    QSqlDatabase db;
};

#endif // DATABASE_H
