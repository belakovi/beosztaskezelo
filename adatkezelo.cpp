#include "adatkezelo.h"
#include "ui_adatkezelo.h"
#include <QMessageBox>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include "dbmanager.h"

#define MAXROW 22
#define MAXCOLUMN 3

Adatkezelo::Adatkezelo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Adatkezelo)
{
    ui->setupUi(this);
    adatbazis = new DbManager();

    model = new QStandardItemModel(MAXROW, MAXCOLUMN, this);
    model->setHorizontalHeaderLabels({"Név", "Részleg", "Muszak" , "email"});

    if (!adatbazis->getAllRecord())
    {
        QStringList oneRow = adatbazis->getNextRecord();
        int tableRow = 0;
        while (!oneRow.isEmpty())
        {
            QStandardItem *nev = new QStandardItem(oneRow.at(0));
            model->setItem(tableRow, 0, nev);
            QStandardItem *reszleg = new QStandardItem(oneRow.at(1));
            model->setItem(tableRow, 1, reszleg);
            QStandardItem *muszak = new QStandardItem(oneRow.at(2));
            model->setItem(tableRow, 2, muszak);
            QStandardItem *email = new QStandardItem(oneRow.at(3));
            model->setItem(tableRow, 3, email);
            tableRow++;
            oneRow = adatbazis->getNextRecord();
        }
    }
    ui->tableView->setModel(model);
}

Adatkezelo::~Adatkezelo()
{
    delete ui;
    delete adatbazis;
}

void Adatkezelo::on_ButtonCancel_clicked()
{
    Adatkezelo::close();
}

bool Adatkezelo::checkSameNameInTable()
{
    QModelIndex refModelIndex, modelIndex;
    int maxRows = model->rowCount();
    for (int row=0; row<maxRows; row++)
    {
        refModelIndex = model->index(row, 0, QModelIndex());
        for (int rest=row+1; rest<maxRows; rest++)
        {
            modelIndex = model->index(rest, 0, QModelIndex());
            if (ui->tableView->model()->data(modelIndex).toString() != "" &&
                ui->tableView->model()->data(modelIndex).toString() ==
                ui->tableView->model()->data(refModelIndex).toString())
            {
                QMessageBox msgBox;
                QString hiba("Nem lehet ugyanaz a nev: %1");
                msgBox.setText(hiba.arg(ui->tableView->model()->data(modelIndex).toString()));
                msgBox.exec();
                return true;
            }
        }
    }
    return false;
}

void Adatkezelo::on_ButtonSave_clicked()
{
    if (checkSameNameInTable())
    {
        return;
    }

    if (adatbazis->clearTable() == SUCCESS)
    {
        QStringList record;
        QModelIndex index;
        for (int row=0; row<model->rowCount(); row++)
        {
            record.clear();
            index = model->index(row, 0, QModelIndex());
            record << ui->tableView->model()->data(index).toString();
            index = model->index(row, 1, QModelIndex());
            record << ui->tableView->model()->data(index).toString();
            index = model->index(row, 2, QModelIndex());
            record << ui->tableView->model()->data(index).toString();
            index = model->index(row, 3, QModelIndex());
            record << ui->tableView->model()->data(index).toString();
            if (!record.at(0).isEmpty())
            {
                adatbazis->addRecord(record);
            }
        }
    }
    Adatkezelo::close();
}
