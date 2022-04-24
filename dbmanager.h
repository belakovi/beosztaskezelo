#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include "dbmodel.h"

using namespace std;

#define SUCCESS 0
#define PROBLEM 1

class DbManager
{

public:
    DbManager();
    ~DbManager();
    int clearTable();
    int addDolgozo(DbRecord data);
    void findPerson(QString nev);
    int getRecordCount();
    QStringList getAllDolgozo(bool first);
    int createBeosztasTable(QString year);
    QString getDolgozoBeosztas(QString dbNev, int id);
    void addBeosztasRecord(QString dbNev, DbBeosztas &beosztas);

private:
    QSqlDatabase db;
    QSqlQuery *query;
};

#endif // DBMANAGER_H
