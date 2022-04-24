#include "general.h"
#include "ui_general.h"

#include <QTableView>
#include <QMessageBox>
#include <QComboBox>
#include <QDate>
#include <QCalendar>
#include <QtDebug>
#include <QPrinter>
#include <QPdfWriter>

General::General(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::General)
{    
    adatbazis = new DbManager();
    QStringList oneRow;
    oneRow=adatbazis->getAllDolgozo(true);
    DbRecord dbRecord;
    while(!oneRow.empty())
    {
        oneRow = adatbazis->getAllDolgozo(false);
        if(!oneRow.empty())
        {
            dbRecord.id  = oneRow.at(0).toInt();
            dbRecord.nev = oneRow.at(1);
            dbRecord.reszleg = oneRow.at(2);
            dbRecord.email = oneRow.at(3);
            dolgozok.push_back(dbRecord);
        }
    }

    myModel = new MyModel();
    ui->setupUi(this);
    ui->naptar->setStyleSheet("QHeaderView::section { background-color:gray }");

    ui->Ev->addItems({"2021", "2022", "2023", "2024", "2025"});
    ui->Ev->setCurrentIndex(QDate::currentDate().year()-2021);
    ui->Honap->addItems(honapok);
    ui->Honap->setMinimumWidth(ui->Honap->minimumSizeHint().width());
    ui->Honap->setCurrentIndex(QDate::currentDate().month()-1);
    ui->muszakCombo->addItems(muszakok);
    ui->muszakCombo->setCurrentIndex(0);
    ui->reszleg->addItems(reszlegek);
    ui->reszleg->setCurrentIndex(0);
    ui->naptar->setModel(myModel);
    ui->naptar->show();

    for (list<DbRecord>::iterator it=dolgozok.begin(); it!=dolgozok.end(); ++it)
    {
        if (it->reszleg == ui->reszleg->currentText())
            ui->dolgozo->addItem(it->nev);
    }

    for (int i=0; i<31; i++)
        nevPerNap.push_back("");
    updateBeosztas();
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

void General::loadBeosztas()
{
    beosztas_t actBeosztas;
    beosztas.clear();
    for (list<DbRecord>::iterator it=dolgozok.begin(); it!=dolgozok.end(); ++it)
    {
        if (it->reszleg == ui->reszleg->currentText())
        {
            actBeosztas.nev = it->nev;
            actBeosztas.beosztas.id = it->id;
            actBeosztas.beosztas.ev = adatbazis->getDolgozoBeosztas(beosztasTablaNev, it->id);
            if (actBeosztas.beosztas.ev.isEmpty())
            {
                actBeosztas.beosztas.ev = actBeosztas.beosztas.ev.leftJustified(MAX_NAPOK, NOT_USED);
            }
            beosztas.push_back(actBeosztas);
        }
    }
}

void General::createBeosztas(DbRecord &dolgozok, DbBeosztas &oneRecord)
{
/*    oneRecord.nev = dolgozok.nev;
    QDate startDate = QDate::fromString(dolgozok.date,"yyyy-MM-dd");
    int startWeek = startDate.weekNumber();
    oneRecord.hetiBeosztas.fill(' ', 54);
    oneRecord.hetiBeosztas[startWeek] = BEOSZTAS_CODE(dolgozok.muszak);
    for (int week=startWeek+1; week<53; week++)
    {
        oneRecord.hetiBeosztas[week] = OPPOSITE_BEOSZTAS(oneRecord.hetiBeosztas[week-1]);
    }*/
}

void General::generateApolok(int startDay, int endDay)
{
    QChar actMuszak;
    int i;

    for (list<beosztas_t>::iterator it=beosztas.begin(); it!=beosztas.end(); ++it)
    {
        if (it->beosztas.ev.at(startDay)==NOT_USED)
        {
            actMuszak = NAPPALI;
            for (i=startDay; i<MAX_NAPOK-1; i+=2)
            {
                it->beosztas.ev.replace(i, 1, actMuszak);
                it->beosztas.ev.replace(i+1, 1, PIHENO);
                actMuszak = (actMuszak==NAPPALI?EJSZAKAI:NAPPALI);
            }
            if (i<MAX_NAPOK)
                it->beosztas.ev.replace(i, 1, PIHENO);
        }

        for (int i=0; i<=endDay; i++)
        {
            if ((it->beosztas.ev.at(startDay+i)==NAPPALI && ui->muszakCombo->currentText()==muszakok[0]) ||
                (it->beosztas.ev.at(startDay+i)==EJSZAKAI && ui->muszakCombo->currentText()==muszakok[1]))
            {
                nevPerNap[i] += it->nev +"\n";
            }
        }
    }
}

void General::generateKarbantarto()
{

}

void General::generateOrvos()
{

}

void General::generatePortas()
{

}

void General::generateTakarito()
{

}

void General::updateBeosztas()
{
    if(nevPerNap.isEmpty())
        return;

    myModel->clearCellsText();

    for (int i=0; i<31; i++)
        nevPerNap[i].clear();

    QDate currentDay, firstDay;
    currentDay.setDate(ui->Ev->currentText().toInt(), ui->Honap->currentIndex()+1, 1);
    firstDay.setDate(ui->Ev->currentText().toInt(), 1, 1);
    int startDay = firstDay.daysTo(currentDay);

    switch (reszlegek.indexOf(ui->reszleg->currentText()))
    {
    case 0:
        generateApolok(startDay, currentDay.daysInMonth());
        break;
    case 1:
        generateKarbantarto();
        break;
    case 2:
        generateOrvos();
        break;
    case 3:
        generatePortas();
        break;
    case 4:
        generateTakarito();
        break;

    }

    int maxRow = myModel->rowCount();
    int maxCol = myModel->columnCount();
    int index = 0;
    for (int row=0; row<maxRow; row++)
    {
        for (int col=0; col<maxCol; col++)
        {
            if (index==0 && myModel->getCellDay(row, col)==1)
                index++;
            if(index && index<=currentDay.daysInMonth())
                myModel->setCellText(row, col, nevPerNap[index++]);
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
    beosztasTablaNev = "beosztas"+arg1;
    updateTableSettings(arg1, ui->Honap->currentText());
    updateBeosztas();
}

void General::on_reszleg_currentTextChanged(const QString &arg1)
{
    ui->dolgozo->clear();
    ui->pdfCcombo->clear();
    ui->pdfCcombo->addItem("Egész részleg");
    for (list<DbRecord>::iterator it=dolgozok.begin(); it!=dolgozok.end(); ++it)
    {
        if (it->reszleg == arg1)
        {
            ui->dolgozo->addItem(it->nev);
            ui->pdfCcombo->addItem(it->nev);
        }
    }
    updateTableSettings(ui->Ev->currentText(), ui->Honap->currentText());
    loadBeosztas();
    updateBeosztas();
}

void General::on_muszakCombo_currentTextChanged(const QString &arg1)
{
    updateTableSettings(ui->Ev->currentText(), ui->Honap->currentText());
    updateBeosztas();
}

void General::on_pdfButton_clicked()
{
    if (ui->pdfCcombo->currentIndex() == 0)
        createReszlegPdf(ui->reszleg->currentText());
    else
        createDolgozoPdf(ui->pdfCcombo->currentText());

}

void General::createReszlegPdf(QString reszleg)
{
    QTextDocument doc;
    QString text;
    QStringList tmp;
    QDate currentDay;

    text = QStringLiteral("<p align=center> <b><font size=+12>%1 %2 beosztás</font></b></p>").arg(ui->reszleg->currentText(), ui->Honap->currentText());
    if (ui->reszleg->currentText() == reszlegek[0]) //apolo
        text += QStringLiteral("<p align=center> <b><font size=+12>muszak: %1</font></b></p>").arg(ui->muszakCombo->currentText());

    currentDay.setDate(ui->Ev->currentText().toInt(), ui->Honap->currentIndex()+1, 1);
    for (int i=1; i<=currentDay.daysInMonth(); i++)
    {
        text += QStringLiteral("<p><b>%1 %2</b></p>").arg(ui->Honap->currentText()).arg(i);
        if(!nevPerNap[i-1].isEmpty())
        {
            tmp = nevPerNap[i-1].split("\n");
            for (int cnt=0; cnt<tmp.size(); cnt++)
                text += QStringLiteral("<p>%1</p>").arg(tmp[cnt]);
        }
    }

    doc.setHtml(text);
/*minta...
    doc.setHtml( "<p>A QTextDocument can be used to present formatted text "
               "in a nice way.</p>"
               "<p align=center>It can be <b>formatted</b> "
               "<font size=+2>in</font> <i>different</i> ways.</p>"
               "<p>The text can be really long and contain many "
               "paragraphs. It is properly wrapped and such...</p>" );
*/
    QPrinter printer;
    printer.setOutputFileName(reszleg+ui->Ev->currentText()+ui->Honap->currentText()+".pdf");
    printer.setOutputFormat(QPrinter::PdfFormat);
    doc.print(&printer);
    printer.newPage();
}

void General::createDolgozoPdf(QString dolgozo)
{
    QTextDocument doc;
    QString text;
    QDate currentDay, firstDay;
    currentDay.setDate(ui->Ev->currentText().toInt(), ui->Honap->currentIndex()+1, 1);
    firstDay.setDate(ui->Ev->currentText().toInt(), 1, 1);
    int startDay = firstDay.daysTo(currentDay);

    text = QStringLiteral("<p align=center> <b><font size=+12>%1 %2 beosztás</font></b></p>").arg(ui->pdfCcombo->currentText(), ui->Honap->currentText());

    for (list<beosztas_t>::iterator it=beosztas.begin(); it!=beosztas.end(); ++it)
    {
        if (it->nev == dolgozo)
        {
            for (int i=0; i<currentDay.daysInMonth(); i++)
            {
                text += QStringLiteral("<p><b>%1 %2</b></p>").arg(ui->Honap->currentText()).arg(i+1);
                switch (it->beosztas.ev.at(startDay+i).unicode()) {
                case NAPPALI:
                    text += QStringLiteral("<p>Nappali muszak</p>");
                    break;
                case EJSZAKAI:
                    text += QStringLiteral("<p>Éjszakai muszak</p>");
                    break;
                case PIHENO:
                    text += QStringLiteral("<p>Pihenö nap</p>");
                    break;
                }
            }
            break;
        }
    }

    doc.setHtml(text);
    QPrinter printer;
    printer.setOutputFileName(dolgozo+ui->Ev->currentText()+ui->Honap->currentText()+".pdf");
    printer.setOutputFormat(QPrinter::PdfFormat);
    doc.print(&printer);
    printer.newPage();
}

void General::on_ValtoztatButton_clicked()
{

}


void General::on_MentButton_clicked()
{
    for (list<beosztas_t>::iterator it=beosztas.begin(); it!=beosztas.end(); ++it)
    {
        adatbazis->addBeosztasRecord(beosztasTablaNev, it->beosztas);
    }
}

