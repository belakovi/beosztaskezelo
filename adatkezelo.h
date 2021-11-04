#ifndef ADATKEZELO_H
#define ADATKEZELO_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFile>
#include <filekezelo.h>



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

private:
    Ui::Adatkezelo *ui;
    QStandardItemModel *model;
    filekezelo *adatbazisFile;
    QStringList m_TableHeader;
};

#endif // ADATKEZELO_H
