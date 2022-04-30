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

int DbManager::inquiryFreeDays(QString currentYear)
{
    query = new QSqlQuery(QString("SELECT * FROM munkanapValtozas WHERE datum LIKE '%1-%'").arg(currentYear));

    if (!query->exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (inquiryFreeDays): %1");
        msgBox.setText(hiba.arg(query->lastError().text()));
        msgBox.exec();
        delete query;
        return PROBLEM;
    }

    return SUCCESS;
}

QStringList DbManager::getFreeDays()
{
    QStringList oneRecord = {};
    if( query->next() )
    {
        oneRecord << query->value(0).toString();
        oneRecord << query->value(1).toString();
    }
    else
    {
        delete query;
    }
    return oneRecord;
}

QStringList DbManager::getAllRecord(bool first, QString tableName)
{
    QStringList oneRecord = {};

    if (first)
    {
        query = new QSqlQuery(QString("SELECT * FROM %1").arg(tableName));
        if (!query->exec())
        {
            QMessageBox msgBox;
            QString hiba("Adatbazis hiba (getAllRecord, %2): %1");
            msgBox.setText(hiba.arg(tableName, query->lastError().text()));
            msgBox.exec();
            delete query;
        }
        else
            oneRecord << "OK";
    }
    else
    {
        if (tableName == "dolgozok")
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
        else
        {
            if( query->next() )
            {
                oneRecord << query->value(0).toString();
                oneRecord << query->value(1).toString();
            }
            else
            {
                delete query;
            }
        }
    }

    return oneRecord;
}

int DbManager::getDolgozokRecordCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM dolgozok");
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int DbManager::getNapokRecordCount()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM munkanapValtozas");
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int DbManager::clearTable(QString tableName)
{
    QSqlQuery query(QString("DELETE FROM %1").arg(tableName));
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

int DbManager::addDolgozo(DbDolgozoRecord szemelyAdat)
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
       QString hiba("Adatbazis hiba (addRecord dolgozo): %1");
       msgBox.setText(hiba.arg(query.lastError().text()));
       msgBox.exec();
       return PROBLEM;
   }
   return SUCCESS;
}

int DbManager::addNapok(DbNapRecord data)
{
    QSqlQuery query;

    query.prepare("INSERT INTO munkanapValtozas (datum, type) "
                  "VALUES (:date, :stat)");
    query.bindValue(":date", data.datum);
    if(data.status==napStatus[0])
        query.bindValue(":stat", MUNKANAP);
    else
        query.bindValue(":stat", SZABADNAP);
    if(!query.exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (addRecord nap): %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
        return PROBLEM;
    }
    return SUCCESS;
}

int DbManager::createBeosztasTable(QString year)
{
    QString sqlCommand = QString("CREATE TABLE IF NOT EXISTS beosztas%1 (id    INTEGER      PRIMARY KEY, "
                                 "napok STRING (366))").arg(year);
    QSqlQuery query(sqlCommand);
    if (!query.exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (createBeosztasTable): %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
        return PROBLEM;
    }
    return SUCCESS;
}

void DbManager::addBeosztasRecord(QString dbNev, DbBeosztas &beosztas)
{
    QSqlQuery query;
    QString sqlCommand = QString("INSERT or REPLACE INTO %1 (id, napok) VALUES (:id, :napok)").arg(dbNev);
    query.prepare(sqlCommand);
    query.bindValue(":id", beosztas.id);
    query.bindValue(":napok", beosztas.ev);
    if(!query.exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (addBeosztasRecord): %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
    }
}

void DbManager::updateBeosztasRecord(QString dbNev, DbBeosztas &beosztas)
{
    QString sqlCommand = QString("UPDATE %1 SET napok = %2 WHERE id = %3").arg(dbNev, beosztas.ev, QString::number(beosztas.id));

    QSqlQuery query;
    query.prepare(sqlCommand);
    if(!query.exec())
    {
        QMessageBox msgBox;
        QString hiba("Adatbazis hiba (updateBeosztasRecord): %1");
        msgBox.setText(hiba.arg(query.lastError().text()));
        msgBox.exec();
    }
}

QString DbManager::getDolgozoBeosztas(QString dbNev, int id)
{
    QString sqlCommand = QString("SELECT napok FROM %1 WHERE id='%2'").arg(dbNev, QString::number(id));
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
