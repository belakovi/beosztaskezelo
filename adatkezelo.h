#ifndef ADATKEZELO_H
#define ADATKEZELO_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <dbmanager.h>

namespace Ui {
class Adatkezelo;
}

class Adatkezelo : public QDialog
{
    Q_OBJECT

public:
    explicit Adatkezelo(QWidget *parent = nullptr);
    ~Adatkezelo();


private slots:
    void on_ButtonSave_clicked();
    void on_ButtonCancel_clicked();
    bool checkSameNameInTable();

private:
    Ui::Adatkezelo *ui;
    QTableView *table;
    QStandardItemModel *model;
    DbManager *adatbazis;
};

#endif // ADATKEZELO_H
