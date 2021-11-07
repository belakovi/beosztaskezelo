#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
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
        QString hiba("Adatbazis hiba: %1");
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
        oneRecord << query->value(1).toString();
        oneRecord << query->value(2).toString();
        oneRecord << query->value(3).toString();
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
        QString hiba("Adatbazis hiba: %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
        return PROBLEM;
    }
    return SUCCESS;
}

int DbManager::addRecord(int id, QStringList szemelyAdat)
{
   QSqlQuery query;

   query.prepare("INSERT INTO beosztas (id, nev, muszak, email) "
                 "VALUES (:id, :name, :beosztas, :email)");
   query.bindValue(":id", id);
   query.bindValue(":name", szemelyAdat.at(0));
   query.bindValue(":beosztas", szemelyAdat.at(1));
   query.bindValue(":email", szemelyAdat.at(2));
   if(!query.exec())
   {
       QMessageBox msgBox;
       QString hiba("Adatbazis hiba: %1");
       msgBox.setText(hiba.arg(query.lastError().text()));
       msgBox.exec();
       return PROBLEM;
   }
   return SUCCESS;
}
