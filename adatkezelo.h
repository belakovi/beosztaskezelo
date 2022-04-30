#ifndef ADATKEZELO_H
#define ADATKEZELO_H

#include <QDialog>
#include <dbmanager.h>
#include "dolgozomodel.h"

namespace Ui {
class Adatkezelo;
}

class Adatkezelo : public QDialog
{
    Q_OBJECT

public:
    explicit Adatkezelo(QWidget *parent = nullptr);
    ~Adatkezelo();
    bool checkSameNameInTable();


private slots:
    void on_ButtonSave_clicked();
    void on_ButtonCancel_clicked();

private:
    Ui::Adatkezelo *ui;
    dolgozoModel *myModel;
    DbManager *adatbazis;
};

#endif // ADATKEZELO_H
