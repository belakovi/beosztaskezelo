#ifndef GENERAL_H
#define GENERAL_H

#include <QDialog>
#include <QStandardItemModel>
#include "dbmanager.h"
#include "mymodel.h"


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
    void updateTableSettings(QString ev, QString honap);
    void loadBeosztas();

    void generateApolok(int startDay, int endDay);
    void generateKarbantarto();
    void generateOrvos();
    void generatePortas();
    void generateTakarito();

    void createReszlegPdf(QString reszleg);
    void createDolgozoPdf(QString reszleg);


private slots:
    void on_Honap_currentTextChanged(const QString &arg1);

    void on_Ev_currentTextChanged(const QString &arg1);

    void on_reszleg_currentTextChanged(const QString &arg1);

    void on_muszakCombo_currentTextChanged(const QString &arg1);

    void on_pdfButton_clicked();

    void on_ValtoztatButton_clicked();

    void on_MentButton_clicked();

private:
    Ui::General *ui;
    QStandardItemModel *model;
    DbManager *adatbazis;
    QString beosztasTablaNev = "";
    list<DbRecord> dolgozok;
    list<beosztas_t> beosztas;
    QStringList nevPerNap;
    MyModel *myModel;
};

#endif // GENERAL_H
