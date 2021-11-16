#include "adatkezelo.h"
#include "ui_adatkezelo.h"
#include <QMessageBox>

#include "dbmanager.h"
#include "comboboxitemdelegate.h"
#include "general.h"

#define MAXROW 22
#define MAXCOLUMN 3

Adatkezelo::Adatkezelo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Adatkezelo)
{
    myModel = new DbModel();
    ui->setupUi(this);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:gray }");

    ComboBoxItemDelegate* cbMuszak = new ComboBoxItemDelegate(ui->tableView, muszakok);
    ui->tableView->setItemDelegateForColumn(2, cbMuszak);

    ComboBoxItemDelegate* cbReszleg = new ComboBoxItemDelegate(ui->tableView, reszlegek);
    ui->tableView->setItemDelegateForColumn(1, cbReszleg);

    adatbazis = new DbManager();
    int rowCount = adatbazis->getRecordCount();
    myModel->setRowCount(rowCount+100);
    QStringList oneRow;
    if (!adatbazis->getAllRecord())
    {
        for (int cnt=0; cnt<rowCount; cnt++)
        {
            oneRow = adatbazis->getNextRecord();
            myModel->addTableData(oneRow);
            oneRow.clear();
        }
    }
    ui->tableView->setModel(myModel);
}

Adatkezelo::~Adatkezelo()
{
    delete ui;
    delete adatbazis;
    delete myModel;
}

void Adatkezelo::on_ButtonCancel_clicked()
{
    Adatkezelo::close();
}

void Adatkezelo::on_ButtonSave_clicked()
{
    QString name;
    if ((name=myModel->checkSameName())!="")
    {
        QMessageBox msgBox;
        QString hiba("Nem lehet ugyanaz a nev: %1");
        msgBox.setText(hiba.arg(name));
        msgBox.exec();
        return;
    }

    if (adatbazis->clearTable() == SUCCESS)
    {
        int index = 0;
        DbRecord oneRowData = myModel->getOneRowData(index++);
        while (oneRowData.nev != "")
        {
            adatbazis->addRecord(oneRowData);
            oneRowData = myModel->getOneRowData(index++);
        }
    }
    Adatkezelo::close();
}
