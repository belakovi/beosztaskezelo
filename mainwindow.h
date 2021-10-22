#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adatkezelo.h"
#include "general.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
   /* void on_actionAdatok_triggered();*/

    void on_adatok_button_clicked();

    void on_beosztas_button_clicked();

private:
    Ui::MainWindow *ui;
    Adatkezelo *adatkezelo;
    General *general;
};
#endif // MAINWINDOW_H
