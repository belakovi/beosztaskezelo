#ifndef GENERAL_H
#define GENERAL_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class General;
}

class General : public QDialog
{
    Q_OBJECT

public:
    explicit General(QWidget *parent = nullptr);
    ~General();

private:
    Ui::General *ui;
    QStandardItemModel *model;
};

#endif // GENERAL_H
