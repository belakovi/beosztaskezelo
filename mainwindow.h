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

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
   /* void on_actionAdatok_triggered();*/

    void on_adatok_button_clicked();

    void on_beosztas_button_clicked();

    void on_actionAdatok_2_triggered();

    void on_actionBeosztas_triggered();

private:
    Ui::MainWindow *ui;
    Adatkezelo *adatkezelo;
    General *general;
};
#endif // MAINWINDOW_H
