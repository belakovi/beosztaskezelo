#include "adatkezelo.h"
#include "ui_adatkezelo.h"
#include <QMessageBox>
#include <QTableView>
#include <QTableWidget>
#include <QComboBox>
#include "filekezelo.h"
#include "comboboxitemdelegate.h"

Adatkezelo::Adatkezelo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Adatkezelo)
{
    ui->setupUi(this);
 /*
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
*/
    ComboBoxItemDelegate* cbid = new ComboBoxItemDelegate(ui->tableWidget);
    // ComboBox only in column 2
    ui->tableWidget->setItemDelegateForColumn(1, cbid);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(200);
    m_TableHeader<<"Név"<<"Beosztás"<<"Email";
    ui->tableWidget->setHorizontalHeaderLabels(m_TableHeader);

    //connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));

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
/*
    for (int row = 0; row < model->rowCount(); row++)
    {
        for (int col = 0; col < model->columnCount(); col++)
        {
            text += ui->tableView->model()->index(row, col).data().toString() + "#";
        }
    }
    adatbazisFile->overwritefile(text);
*/
    Adatkezelo::close();
}
