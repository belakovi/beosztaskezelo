#ifndef GENERAL_H
#define GENERAL_H

#include <QDialog>
#include <QStandardItemModel>
#include "dbmanager.h"

typedef struct {
    QString nev;
    QString muszak;
    QString email;
} DbRecord;

const QStringList honapok = {"Január", "Február", "Március", "Április", "Május", "Június",
                             "Július", "Augusztus", "Szeptember", "Október", "November", "December"};
const QStringList napok = {"Hétfő" , "Kedd", "Szerda", "Csütörtök", "Péntek", "Szombat", "Vasárnap"};

namespace Ui {
class General;
}

class General : public QDialog
{
    Q_OBJECT

public:
    explicit General(QWidget *parent = nullptr);
    ~General();
    int getWeekNumbers(int year, int month);

private slots:
    void on_Honap_currentTextChanged(const QString &arg1);

private:
    Ui::General *ui;
    QStandardItemModel *model;
    DbManager *adatbazis;
    list<DbRecord> dbRecords;
};

#endif // GENERAL_H
