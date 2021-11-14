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
    int addRecord(DbData data);
    void findPerson(QString nev);
    int getRecordCount();
    int getAllRecord();
    QStringList getNextRecord();

private:
    QSqlDatabase db;
    QSqlQuery *query;
};

#endif // DBMANAGER_H
