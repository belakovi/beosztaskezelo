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
    int addRecord(DbRecord data);
    void findPerson(QString nev);
    int getRecordCount();
    int getAllRecord();
    QStringList getNextRecord();
    int createBeosztasTable(QString year);
    int collectBeosztasok(QString dbNev, QString nev, DbBeosztas &beosztas);
    void addBeosztasRecord(QString dbNev, DbBeosztas &beosztas);

private:
    QSqlDatabase db;
    QSqlQuery *query;
};

#endif // DBMANAGER_H
