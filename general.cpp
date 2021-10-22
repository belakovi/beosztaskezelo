#include "general.h"
#include "ui_general.h"

#include <QTableView>
#include <QTextStream>
#include <QMessageBox>
#include "filekezelo.h"


General::General(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::General)
{
    ui->setupUi(this);
    model = new QStandardItemModel(200,3);
    model->setHeaderData(0, Qt::Horizontal, "Hét");
    model->setHeaderData(1, Qt::Horizontal, "Reggel");
    model->setHeaderData(2, Qt::Horizontal, "Este");

    ui->beosztas->horizontalHeader()->setStyleSheet( "QHeaderView::section { border-bottom: 1px solid gray; background-color: black; color: white}");
    ui->beosztas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->beosztas->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    ui->beosztas->sortByColumn(0, Qt::AscendingOrder);

    ui->beosztas->setModel(model);

    filekezelo adatbazisfile;
    if (adatbazisfile.filenyitas("./adatbazis.txt") == EMPTYFILE)
    {
        QMessageBox msgBox;
        msgBox.setText("Az adatbázis üres.");
        msgBox.exec();
    }

    //connect(ui->beosztas, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));

}

General::~General()
{
    delete ui;
}
