#ifndef MUNKANAPVALT_H
#define MUNKANAPVALT_H

#include <QDialog>
#include <dbmanager.h>
#include "napmodel.h"

namespace Ui {
class MunkanapValt;
}

class MunkanapValt : public QDialog
{
    Q_OBJECT

public:
    explicit MunkanapValt(QWidget *parent = nullptr);
    ~MunkanapValt();

private slots:
    void on_cancelButton_clicked();
    void on_mentButton_clicked();

private:
    Ui::MunkanapValt *ui;
    napmodel *myModel;
    DbManager *adatbazis;
};

#endif // MUNKANAPVALT_H
