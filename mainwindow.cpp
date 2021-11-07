#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adatkezelo.h"
#include "general.h"
#include "dbmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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

