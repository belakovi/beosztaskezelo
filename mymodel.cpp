#include "mymodel.h"

MyModel::MyModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void MyModel::setRowCount(int row)
{
    numberOfRow = row;
}

void MyModel::setColCount(int col)
{
    numberOfColumn = col;
}

int MyModel::rowCount(const QModelIndex & /*parent*/) const
{
   return numberOfRow;
}

int MyModel::columnCount(const QModelIndex & /*parent*/) const
{
    return numberOfColumn;
}

void MyModel::setColumnHeader(int column, QString cHeader)
{
    columnHeader[column] = cHeader;
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Hétfő");
        case 1:
            return QString("Kedd");
        case 2:
            return QString("Szerda");
        case 3:
            return QString("Csütörtök");
        case 4:
            return QString("Péntek");
        case 5:
            return QString("Szombat");
        case 6:
            return QString("Vasárnap");
        }
    }

    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return columnHeader[section];
    }
    return QVariant();
}

void MyModel::clearTable()
{
    beginResetModel();
    numberOfRow = 0;
    numberOfColumn = 0;
    memset (columnHeader, 0, sizeof (columnHeader));
    memset (cellData, 0, sizeof (cellData));
    endResetModel();
}

void MyModel::setCellDay(int row, int col, int data)
{
    cellData[row][col].day = data;
}

void MyModel::setCellText(int row, int col, QString data)
{
    cellData[row][col].text = data;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch (role) {
    case Qt::DisplayRole:
        return QString("%1\n%2").arg(cellData[row][col].day).arg(cellData[row][col].text);
    }

    return QVariant();
}
