#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adatkezelo.h"
#include "general.h"
#include "dbmanager.h"
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap bkgnd("/dani/beosztaskezelo/hatter.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{

    QPixmap bkgnd("/dani/beosztaskezelo/hatter.jpg");
    bkgnd = bkgnd.scaled(ev->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);

    QWidget::resizeEvent(ev);
}

/*void MainWindow::on_actionAdatok_triggered()
{
    adatkezelo = new Adatkezelo(this);
    adatkezelo->show();
    //Adatkezelo adatkezelo;
    //adatkezelo.setModal(true);
    //adatkezelo.exec();
}*/

void MainWindow::on_adatok_button_clicked()
{
    adatkezelo = new Adatkezelo(this);
    adatkezelo->show();
}


void MainWindow::on_beosztas_button_clicked()
{
    general = new General(this);
    general->show();
}


void MainWindow::on_actionDolgozok_triggered()
{
    adatkezelo = new Adatkezelo(this);
    adatkezelo->show();
}


void MainWindow::on_actionBeosztas_triggered()
{
    general = new General(this);
    general->show();
}


void MainWindow::on_actionMunkanapValtozas_triggered()
{
    munkanapValt = new MunkanapValt(this);
    munkanapValt->show();
}

