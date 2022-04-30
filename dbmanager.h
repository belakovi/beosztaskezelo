#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include "dolgozomodel.h"
#include "napmodel.h"

using namespace std;

#define SUCCESS 0
#define PROBLEM 1

#define NOT_USED 'F'
#define NAPPALI  'N'
#define EJSZAKAI 'E'
#define PIHENO   'P'
#define TAVOLLET 'T'
#define NORMAL   'A'

#define MAX_NAPOK  366
#define OOO "Távollét" //OOO -> Out Of Office

const QStringList honapok = {"Január", "Február", "Március", "Április", "Május", "Június",
                             "Július", "Augusztus", "Szeptember", "Október", "November", "December"};
const QStringList apoloMuszak = {"Nappali", "Éjszakai"};
const QStringList reszlegek = {"Ápoló", "Karbantartó", "Orvos", "Portás", "Takarító"};
const QStringList aMuszakok = {"Nappali", "Éjszakai", OOO};
const QStringList eMuszakok = {"Normál", OOO};


class DbManager
{

public:
    DbManager();
    ~DbManager();
    int clearTable(QString tableName);
    int addDolgozo(DbDolgozoRecord data);
    int addNapok(DbNapRecord data);
    void findPerson(QString nev);
    int getDolgozokRecordCount();
    int getNapokRecordCount();
    QStringList getAllRecord(bool first, QString tableName);
    int createBeosztasTable(QString year);
    QString getDolgozoBeosztas(QString dbNev, int id);
    void addBeosztasRecord(QString dbNev, DbBeosztas &beosztas);
    void updateBeosztasRecord(QString dbNev, DbBeosztas &beosztas);
    int inquiryFreeDays(QString year);
    QStringList getFreeDays();

private:
    QSqlDatabase db;
    QSqlQuery *query;
};

#endif // DBMANAGER_H
