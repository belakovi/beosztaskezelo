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

namespace Ui {
class General;
}

class General : public QDialog
{
    Q_OBJECT

public:
    explicit General(QWidget *parent = nullptr);
    ~General();

private:
    Ui::General *ui;
    QStandardItemModel *model;
    DbManager *adatbazis;
    list<DbRecord> *dbRecords;
};

#endif // GENERAL_H
