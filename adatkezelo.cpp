#include "adatkezelo.h"
#include "ui_adatkezelo.h"
#include <QMessageBox>
#include <QTableView>
#include <QComboBox>
#include "filekezelo.h"

Adatkezelo::Adatkezelo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Adatkezelo)
{
    ui->setupUi(this);
    model = new QStandardItemModel(200,3);
    model->setHeaderData(0, Qt::Horizontal, "Név");
    model->setHeaderData(1, Qt::Horizontal, "Beosztás");
    model->setHeaderData(2, Qt::Horizontal, "Email");

    ui->tableView->horizontalHeader()->setStyleSheet( "QHeaderView::section { border-bottom: 1px solid gray; background-color: black; color: white}");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);

    adatbazisFile = new filekezelo;
    if (adatbazisFile->filenyitas("./adatbazis.txt") == EMPTYFILE)
    {
        QMessageBox msgBox;
        msgBox.setText("Az adatbázis üres.");
        msgBox.exec();
    }
    else
    {
        QString text = adatbazisFile->fileolvasas();
        QStringList cells = text.split("#");
        QModelIndex index;

        for (int row = 0; row < model->rowCount() && cells.size()-1 > row*model->columnCount(); row++)
        {
            for (int col = 0; col < model->columnCount(); col++)
            {
                if (cells.at(row*model->columnCount()+col) != "")
                {
                    index = model->index(row, col);
                    model->setData(index, cells.at(row*model->columnCount()+col));
                }
            }
        }
    }
    ui->tableView->setModel(model);
    QComboBox c;
    c.addItems({"Nappali","Ejszakai"});
    ui->tableView->setIndexWidget(ui->tableView->model()->index(1,2), c);

    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
}

Adatkezelo::~Adatkezelo()
{
    adatbazisFile->closefile();
    delete ui;
}


void Adatkezelo::on_ButtonCancel_clicked()
{
    Adatkezelo::close();
}

void Adatkezelo::on_ButtonSave_clicked()
{
    QString text = "";

    for (int row = 0; row < model->rowCount(); row++)
    {
        for (int col = 0; col < model->columnCount(); col++)
        {
            text += ui->tableView->model()->index(row, col).data().toString() + "#";
        }
    }
    adatbazisFile->overwritefile(text);
}
