#ifndef GENERAL_H
#define GENERAL_H

#include <QDialog>
#include <QStandardItemModel>
#include "dbmanager.h"
#include "mymodel.h"

typedef struct {
    QString nev;
    QString reszleg;
    QString muszak;
    QString date;
    QString email;
} DbRecord;

const QStringList honapok = {"Január", "Február", "Március", "Április", "Május", "Június",
                             "Július", "Augusztus", "Szeptember", "Október", "November", "December"};
const QStringList muszakok = {"Nappali", "Éjszakai"};
const QStringList reszlegek = {"Ápoló", "Karbantartó", "Orvos", "Portás", "Takarító"};

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
    void setDatesInCell(QDate currentDay);
    void updateBeosztas();

private slots:
    void on_Honap_currentTextChanged(const QString &arg1);

    void on_muszakCombo_currentIndexChanged(int index);

    void on_reszleg_currentIndexChanged(int index);


private:
    Ui::General *ui;
    QStandardItemModel *model;
    DbManager *adatbazis;
    list<DbRecord> dbRecords;
    list<DbRecord> filteredRecords;
    QString actMuszak = "", actReszleg = "";
    MyModel *myModel;
};

#endif // GENERAL_H
