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

void MyModel::addRowHeader(QString cHeader)
{
    rowHeader.push_back(cHeader);
}

QString MyModel::getRowHeader(int row)
{
    return rowHeader.at(row);
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return columnHeader.at(section);
    }

    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return rowHeader.at(section);
    }
    return QVariant();
}

void MyModel::clearCellsText()
{
    for (int row=0; row<MAXHET; row++)
    {
        for (int col=0; col<MAXNAP; col++)
        {
            cellData[row][col].text = "";
        }
    }
}

void MyModel::clearTable()
{
    beginResetModel();
    numberOfRow = 0;
    numberOfColumn = 0;
    rowHeader.clear();
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

void MyModel::updateFinished(){
     QModelIndex topLeft = index(0, 0);
     QModelIndex bottomRight = index(numberOfRow, numberOfColumn);
     //beginResetModel();
     //emit  layoutAboutToBeChanged();
     //emit layoutChanged();
     emit dataChanged(topLeft,bottomRight);
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
