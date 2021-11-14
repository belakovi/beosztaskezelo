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
    myModel = new MyModel();
    actMuszak = "";
    actReszleg = "";
    ui->setupUi(this);
    ui->naptar->setStyleSheet("QHeaderView::section { background-color:gray }");

    ui->Ev->addItems({"2021", "2022", "2023", "2024", "2025"});
    ui->Honap->setCurrentIndex(QDate::currentDate().year()-2021);
    ui->Honap->addItems(honapok);
    ui->Honap->setMinimumWidth(ui->Honap->minimumSizeHint().width());
    ui->Honap->setCurrentIndex(QDate::currentDate().month()-1);
    ui->muszakCombo->addItems(muszakok);
    ui->reszleg->addItems(reszlegek);

    adatbazis = new DbManager();
    //get all records from DB
    if (!adatbazis->getAllRecord())
    {
        DbRecord dbRecord;
        QStringList oneRow = adatbazis->getNextRecord();
        while (!oneRow.isEmpty())
        {
            dbRecord.nev = oneRow.at(0);
            dbRecord.reszleg = oneRow.at(1);
            dbRecord.muszak = oneRow.at(2);
            dbRecord.email = oneRow.at(3);
            dbRecords.push_back(dbRecord);
            oneRow = adatbazis->getNextRecord();
        }
    }

    ui->naptar->setModel(myModel);
    ui->naptar->show();
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
    myModel->clearTable();
    QDate currentDay;
    currentDay.setDate(ui->Ev->currentText().toInt(), honapok.indexOf(arg1, 0)+1, 1);
    int rows = getWeekNumbers(currentDay.year(), currentDay.month());
    myModel->setRowCount(rows);
    myModel->setColCount(7); //napok szama egy heten
    ui->naptar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->naptar->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int refWeekNumber = currentDay.weekNumber();
    for (int rowIndex=0; rowIndex<rows; rowIndex++)
    {
        myModel->addRowHeader(QString::number(refWeekNumber+rowIndex));
        if (refWeekNumber >= 53)
            refWeekNumber = 0;
    }
    setDatesInCell(currentDay);
}

void General::setDatesInCell(QDate currentDay)
{
    int col;
    int maxRow = myModel->rowCount();
    int days = 1;
    for (int row=0; row<maxRow; row++)
    {
        if (row==0) col = currentDay.dayOfWeek()-1;
        else col =0;
        if (col) //set previous month's days
        {
            QDate prevMonth = currentDay.addMonths(-1);
            int startDay = prevMonth.daysInMonth()+1-col;
            for (int i=0; i<col; i++)
            {
                myModel->setCellDay(0, i, startDay++);
            }
        }
        //set current month
        while (col<7 && days<=currentDay.daysInMonth())
        {
            myModel->setCellDay(row, col++, days++);
        }
        //set next month
        if (row+1==maxRow)
        {
            days =1;
            while (col<7)
            {
                myModel->setCellDay(row, col++, days++);
            }
        }
    }
}

void General::updateBeosztas()
{
    filteredRecords.clear();
    for (DbRecord const &record : dbRecords)
    {
        if ((record.reszleg == actReszleg || actReszleg == "") &&
            (record.muszak == actMuszak || actMuszak == ""))
        {
            filteredRecords.push_back(record);
        }
    }

    int maxRow = myModel->rowCount();
    int maxCol = myModel->columnCount();
    for (int row=0; row<maxRow; row++)
    {
        for (int col=0; col<maxCol; col++)
        {
            for (DbRecord const &record : filteredRecords)
            {
                myModel->setCellText(row, col, record.nev);
            }
        }
    }
}

void General::on_reszleg_currentTextChanged(const QString &currentReszleg)
{
    actReszleg = currentReszleg;
    updateBeosztas();
}


void General::on_muszakCombo_currentTextChanged(const QString &currentMuszak)
{
    actMuszak = currentMuszak;
    updateBeosztas();
}

