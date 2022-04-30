#include "munkanapvalt.h"
#include "ui_munkanapvalt.h"
#include "dbmanager.h"
#include "comboboxitemdelegate.h"
#include <QMessageBox>

const QString dateFormat = "yyyy-MM-dd";

MunkanapValt::MunkanapValt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MunkanapValt)
{
    ui->setupUi(this);

    myModel = new napmodel();
    ui->setupUi(this);
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:gray }");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ComboBoxItemDelegate* cbNapStatus = new ComboBoxItemDelegate(ui->tableView, napStatus);
    ui->tableView->setItemDelegateForColumn(1, cbNapStatus);

    adatbazis = new DbManager();
    int rowCount = adatbazis->getNapokRecordCount();
    myModel->setRowCount(rowCount+100);
    QStringList oneRow;
    oneRow=adatbazis->getAllRecord(true, "munkanapValtozas");
    while(!oneRow.empty())
    {
        oneRow = adatbazis->getAllRecord(false, "munkanapValtozas");
        if (!oneRow.empty())
            myModel->addTableData(oneRow);
    }
    ui->tableView->setModel(myModel);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
    connect(ui->mentButton, SIGNAL(clicked()), this, SLOT(on_mentButton_clicked()));
}

MunkanapValt::~MunkanapValt()
{
    delete ui;
    delete adatbazis;
    delete myModel;
}

void MunkanapValt::on_cancelButton_clicked()
{
    MunkanapValt::close();
}

void MunkanapValt::on_mentButton_clicked()
{
    QDate datum;
    if (myModel->checkSameDate(datum))
    {
        QMessageBox msgBox;
        QString hiba("Ez a dátum már szerepel: %1");
        msgBox.setText(hiba.arg(datum.toString(dateFormat)));
        msgBox.exec();
        return;
    }

    if (adatbazis->clearTable("munkanapValtozas") == SUCCESS)
    {
        int index = 0;
        DbNapRecord oneRowData;
        while (myModel->getOneRowData(index++, oneRowData))
        {
            if (oneRowData.datum.isValid())
                adatbazis->addNapok(oneRowData);
        }
    }

    MunkanapValt::close();
}

