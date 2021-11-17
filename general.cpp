#include "general.h"
#include "ui_general.h"

#include <QTableView>
#include <QMessageBox>
#include <QComboBox>
#include <QDate>
#include <QCalendar>
#include <QtDebug>

#define MUSZAK(x) (x==muszakok.at(0) ? 'N' : 'E')

General::General(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::General)
{    
    myModel = new MyModel();
    ui->setupUi(this);
    ui->naptar->setStyleSheet("QHeaderView::section { background-color:gray }");

    ui->Ev->addItems({"2021", "2022", "2023", "2024", "2025"});
    ui->Honap->setCurrentIndex(QDate::currentDate().year()-2021);
    ui->Honap->addItems(honapok);
    ui->Honap->setMinimumWidth(ui->Honap->minimumSizeHint().width());
    ui->Honap->setCurrentIndex(QDate::currentDate().month()-1);
    ui->muszakCombo->addItems(muszakok);
    ui->muszakCombo->setCurrentIndex(0);
    ui->reszleg->addItems(reszlegek);
    ui->reszleg->setCurrentIndex(0);

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
            dbRecord.date = oneRow.at(3);
            dbRecord.email = oneRow.at(4);
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

void General::updateTableSettings(QString ev, QString honap)
{
    myModel->clearTable();
    QDate currentDay;
    currentDay.setDate(ev.toInt(), honapok.indexOf(honap, 0)+1, 1);
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

#define BEOSZTAS_CODE(x) (x==muszakok[0] ? 'N' : 'E')
#define OPPOSITE_BEOSZTAS(x) (x=='N' ? 'E': 'N')

void General::createBeosztas(DbRecord &dolgozok, DbBeosztas &oneRecord)
{
    oneRecord.nev = dolgozok.nev;
    QDate startDate = QDate::fromString(dolgozok.date,"yyyy-MM-dd");
    int startWeek = startDate.weekNumber();
    oneRecord.hetiBeosztas.fill(' ', 54);
    oneRecord.hetiBeosztas[startWeek] = BEOSZTAS_CODE(dolgozok.muszak);
    for (int week=startWeek+1; week<53; week++)
    {
        oneRecord.hetiBeosztas[week] = OPPOSITE_BEOSZTAS(oneRecord.hetiBeosztas[week-1]);
    }
}

void General::updateBeosztas()
{
    if (actReszleg=="" || actMuszak=="")
        return;

    myModel->clearCellsText();
    list<DbRecord> filteredRecords;
    for (DbRecord const &record : dbRecords)
    {
        if (record.reszleg == actReszleg && actReszleg != "")
        {
            filteredRecords.push_back(record);
        }
    }

    list<DbBeosztas> dbBeosztas;
    DbBeosztas oneRecord;;
    QString dbNev = QString("beosztas%1").arg(ui->Ev->currentText());
    for (list<DbRecord>::iterator it = filteredRecords.begin(); it!=filteredRecords.end(); ++it)
    {
        if (adatbazis->collectBeosztasok(dbNev, it->nev, oneRecord)==PROBLEM)
            return;

        if (oneRecord.nev == "")
        { //No record was found
            createBeosztas(*it, oneRecord);
            adatbazis->addBeosztasRecord(dbNev, oneRecord);
        }

        dbBeosztas.push_back(oneRecord);
    }

    int maxRow = myModel->rowCount();
    int maxCol = myModel->columnCount();
    for (int row=0; row<maxRow; row++)
    {
        for (int col=0; col<maxCol; col++)
        {
            if (filteredRecords.size()==0)
            {
                myModel->setCellText(row, col, "");
            }
            else
            {
                int rowWeekNumber = myModel->getRowHeader(row).toInt();
                for (DbBeosztas const &record : dbBeosztas)
                {
                    if (record.hetiBeosztas[rowWeekNumber] == MUSZAK(ui->muszakCombo->currentText()))
                    {
                        myModel->setCellText(row, col, record.nev);
                    }
                }
            }
        }
    }
    myModel->updateFinished();
}

void General::on_Honap_currentTextChanged(const QString &arg1)
{
    updateTableSettings(ui->Ev->currentText(), arg1);
    updateBeosztas();
}

void General::on_Ev_currentTextChanged(const QString &arg1)
{
    adatbazis->createBeosztasTable(arg1);
    updateTableSettings(arg1, ui->Honap->currentText());
    updateBeosztas();
}

void General::on_reszleg_currentTextChanged(const QString &arg1)
{
    actReszleg = arg1;
    updateBeosztas();
}

void General::on_muszakCombo_currentTextChanged(const QString &arg1)
{
    actMuszak = arg1;
    updateBeosztas();
}

