#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include <QDate>
#include <QCoreApplication>
#include "dbmanager.h"

DbManager::DbManager()
{
   db = QSqlDatabase::addDatabase("QSQLITE");
   QString pathAndName = "../beosztaskezelo/adatbazis.db";
   //QString pathAndName = QCoreApplication::applicationDirPath() + "/adatbazis.db";
   db.setDatabaseName(pathAndName);

   if (!db.open())
   {
       QMessageBox msgBox;
       msgBox.setText("Error: connection with database failed");
       msgBox.exec();
   }
}

DbManager::~DbManager()
{
    db.close();
}

QStringList DbManager::getAllDolgozo(bool first)
{
    QStringList oneRecord = {};

    if (first)
    {
        query = new QSqlQuery(QString("SELECT * FROM dolgozok"));
        if (!query->exec())
        {
            QMessageBox msgBox;
            QString hiba("Adatbazis hiba (getAllDolgozo): %1");
            msgBox.setText(hiba.arg(query->lastError().text()));
            msgBox.exec();
            delete query;
        }
        else
            oneRecord << "OK";
    }
    else
    {
        if( query->next() )
        {
            oneRecord << query->value(0).toString();
            oneRecord << query->value(1).toString();
            oneRecord << query->value(2).toString();
            oneRecord << query->value(3).toString();
        }
        else
        {
            delete query;
        }
    }

    return oneRecord;
}

int DbManager::getRecordCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM dolgozok");
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int DbManager::clearTable()
{
    QSqlQuery query(QString("DELETE FROM dolgozok"));
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

int DbManager::addDolgozo(DbRecord szemelyAdat)
{
   QSqlQuery query;

   query.prepare("INSERT INTO dolgozok (id, nev, reszleg, email) "
                 "VALUES (:id, :name, :reszleg, :email)");
   query.bindValue(":id", szemelyAdat.id);
   query.bindValue(":name", szemelyAdat.nev);
   query.bindValue(":reszleg", szemelyAdat.reszleg);
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

int DbManager::createBeosztasTable(QString year)
{
/*    QString sqlCommand = QString("CREATE TABLE IF NOT EXISTS beosztas%1 (nev   STRING (40) PRIMARY KEY, "
                                 "het STRING (60))").arg(year);
    QSqlQuery query(sqlCommand);
    if (!query.exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (createBeosztasTable): %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
        return PROBLEM;
    }*/
    return SUCCESS;
}

void DbManager::addBeosztasRecord(QString dbNev, DbBeosztas &beosztas)
{
/*    QSqlQuery query;
    QString sqlCommand = QString("INSERT INTO %1 (nev, het) VALUES (:nev, :beosztas)").arg(dbNev);
    query.prepare(sqlCommand);
    query.bindValue(":nev", beosztas.nev);
    query.bindValue(":beosztas", beosztas.hetiBeosztas);
    if(!query.exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (addBeosztasRecord): %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
    }*/
}

QString DbManager::getDolgozoBeosztas(QString dbNev, int id)
{
    QString sqlCommand = QString("SELECT napok FROM %1 WHERE id='%1'").arg(dbNev, id);
    QSqlQuery query;
    query.prepare(sqlCommand);
    if (!query.exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (collectBeosztasok): %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
        return "";
    }

    if( query.next() )
    {
        return query.value(0).toString();
    }
    return "";
}
