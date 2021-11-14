#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include <QDate>
#include "dbmanager.h"

DbManager::DbManager()
{
   db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("../beosztaskezelo/adatbazis.db");

   if (!db.open())
   {
       QMessageBox msgBox;
       msgBox.setText("Error: connection with database failed");
       msgBox.exec();
   }

   if (getRecordCount() == 0)
   {
       QMessageBox msgBox;
       msgBox.setText("Az adatbazis ures");
       msgBox.exec();
   }
}

DbManager::~DbManager()
{
    db.close();
}

int DbManager::getAllRecord()
{
    query = new QSqlQuery(QString("SELECT * FROM beosztas"));
    if (!query->exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (getAllRecord): %1");
        msgBox.setText(hiba.arg(query->lastError().text()));
        msgBox.exec();
        delete query;
        return PROBLEM;
    }
    return SUCCESS;
}

QStringList DbManager::getNextRecord()
{
    QStringList oneRecord = {};
    if( query->next() )
    {
        oneRecord << query->value(0).toString();
        oneRecord << query->value(1).toString();
        oneRecord << query->value(2).toString();
        oneRecord << query->value(3).toString();
        oneRecord << query->value(4).toString();
    }
    else
    {
        delete query;
    }
    return oneRecord;
}

int DbManager::getRecordCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM beosztas");
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int DbManager::clearTable()
{
    QSqlQuery query(QString("DELETE FROM beosztas"));
    if (!query.exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (clearTable): %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
        return PROBLEM;
    }
    return SUCCESS;
}

int DbManager::addRecord(DbData szemelyAdat)
{
   QSqlQuery query;

   query.prepare("INSERT INTO beosztas (nev, reszleg, muszak, muszak_start, email) "
                 "VALUES (:name, :reszleg, :muszak, :mdate, :email)");
   query.bindValue(":name", szemelyAdat.nev);
   query.bindValue(":reszleg", szemelyAdat.reszleg);
   query.bindValue(":muszak", szemelyAdat.muszak);
   query.bindValue(":mdate", szemelyAdat.startDate);
   query.bindValue(":email", szemelyAdat.email);
   if(!query.exec())
   {
       QMessageBox msgBox;
       QString hiba("Adatbazis hiba (addRecord): %1");
       msgBox.setText(hiba.arg(query.lastError().text()));
       msgBox.exec();
       return PROBLEM;
   }
   return SUCCESS;
}
