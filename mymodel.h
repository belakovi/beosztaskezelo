#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractTableModel>

#define MAXHET 6
#define MAXNAP 7

typedef struct {
    int day;
    QString text;
} CellData;

class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MyModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setRowCount(int row);
    void setColCount(int col);
    void setColumnHeader(int column, QString cHeader);
    void setCellDay(int row, int col, int data);
    void setCellText(int row, int col, QString data);
    void clearTable(void);

private:
    int numberOfRow = 0;
    int numberOfColumn = 0;
    QString columnHeader[7];
    CellData cellData[MAXHET][MAXNAP];
};
#endif // MYMODEL_H
