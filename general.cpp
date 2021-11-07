#include "general.h"
#include "ui_general.h"

#include <QTableView>
#include <QTextStream>
#include <QMessageBox>

General::General(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::General)
{
    ui->setupUi(this);
    adatbazis = new DbManager();

    model = new QStandardItemModel(200,3);
    model->setHeaderData(0, Qt::Horizontal, "Hét");
    model->setHeaderData(1, Qt::Horizontal, "Reggel");
    model->setHeaderData(2, Qt::Horizontal, "Este");

    ui->beosztas->horizontalHeader()->setStyleSheet( "QHeaderView::section { border-bottom: 1px solid gray; background-color: black; color: white}");
    ui->beosztas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->beosztas->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    ui->beosztas->sortByColumn(0, Qt::AscendingOrder);

    ui->beosztas->setModel(model);
    //connect(ui->beosztas, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));

    //get all records from DB
    if (!adatbazis->getAllRecord())
    {
        DbRecord dbRecord;
        QStringList oneRow = adatbazis->getNextRecord();
        while (!oneRow.isEmpty())
        {
            dbRecord.nev = oneRow.at(0);
            dbRecord.muszak = oneRow.at(1);
            dbRecord.email = oneRow.at(2);
            dbRecords->push_back(dbRecord);
            oneRow = adatbazis->getNextRecord();
        }
    }


}

General::~General()
{
    delete ui;
}
