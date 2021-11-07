#include "general.h"
#include "ui_general.h"

#include <QTableView>
#include <QMessageBox>
#include <QComboBox>
#include <QDate>
#include <QCalendar>

General::General(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::General)
{
    ui->setupUi(this);

    ui->Ev->addItems({"2021", "2022", "2023", "2024", "2025"});
    ui->Honap->setCurrentIndex(QDate::currentDate().year()-2021);
    ui->Honap->addItems(honapok);
    ui->Honap->setMinimumWidth(ui->Honap->minimumSizeHint().width());
    ui->Honap->setCurrentIndex(QDate::currentDate().month()-1);

    ui->naptar->setModel(model);

    adatbazis = new DbManager();
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
            dbRecords.push_back(dbRecord);
            oneRow = adatbazis->getNextRecord();
        }
    }
}

General::~General()
{
    delete ui;
}

int General::getWeekNumbers(int startYear, int startMonth)
{
    QDate firstDay;
    firstDay.setDate(startYear, startMonth, 1);
    int i = (firstDay.dayOfWeek()-1)+firstDay.daysInMonth();
    if (i%7)
        return (i/7)+1;
    return i/7;
}

void General::on_Honap_currentTextChanged(const QString &arg1)
{
    QDate currentDay;
    currentDay.setDate(ui->Ev->currentText().toInt(), honapok.indexOf(arg1, 0)+1, 1);
    int rows = getWeekNumbers(currentDay.year(), currentDay.month());
    model = new QStandardItemModel(rows, napok.count(), this);
    model->setHorizontalHeaderLabels(napok);
    ui->naptar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->naptar->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int refWeekNumber = currentDay.weekNumber();
    for (int rowIndex=0; rowIndex<rows; rowIndex++)
    {
        model->setHeaderData(rowIndex, Qt::Vertical, refWeekNumber+rowIndex);
        if (refWeekNumber >= 53)
            refWeekNumber = 0;
    }
    ui->naptar->setModel(model);
    //show days
    int col;
    int days = 1;
    for (int row=0; row<rows; row++)
    {
        if (row==0) col = currentDay.dayOfWeek()-1;
        else col =0;
        while (col<7 && days<=currentDay.daysInMonth())
        {
            QStandardItem *cellItem = new QStandardItem(QString::number(days++));
            model->setItem(row, col++, cellItem);
        }
    }
}

