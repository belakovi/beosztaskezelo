#include "dbmodel.h"
#include <QDate>

DbModel::DbModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void DbModel::setRowCount(int row)
{
    numberOfRow = row;
}

int DbModel::rowCount(const QModelIndex & /*parent*/) const
{
   return numberOfRow;
}

int DbModel::columnCount(const QModelIndex & /*parent*/) const
{
    return columnHeader.size();
}

QVariant DbModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return columnHeader.at(section);
    }

    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return section;
    }
    return QVariant();
}

void DbModel::addTableData(QStringList dbData)
{
    DbRecord data;
    data.nev       = dbData.at(0);
    data.reszleg   = dbData.at(1);
    data.muszak    = dbData.at(2);
    data.date      = dbData.at(3);
    data.email     = dbData.at(4);
    rowData.push_back(data);
}

void DbModel::updateTableData(int rowIndex, DbRecord dbData)
{
    if(rowIndex<0 || rowIndex>(int)rowData.size()) //if index points to invalid place then don't update
        return;
    auto it =  rowData.begin();
    advance(it, rowIndex);
    *it = dbData;
}

void DbModel::clearTable()
{
    beginResetModel();
    numberOfRow = 0;
    rowData.clear();
    endResetModel();
}

void DbModel::addOneRowToTable()
{
    beginResetModel();
    numberOfRow += 1;
    endResetModel();
}

QVariant DbModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        int row = index.row();
        if (row>=(int)rowData.size())
            return QVariant();
        auto it =  rowData.begin();
        advance(it, row);
        switch (index.column()) {
        case 0:
            return it->nev;
        case 1:
            return it->reszleg;
        case 2:
            return it->muszak;
        case 3:
            return it->date;
        case 4:
            return it->email;
        }
    }

    return QVariant();
}

bool compare_nocase (const DbRecord& first, const DbRecord& second)
{
    return (first.nev<second.nev);
}

QString DbModel::checkSameName()
{
    rowData.sort(compare_nocase);
    list<DbRecord>::iterator it = rowData.begin();
    list<DbRecord>::iterator nextIt = rowData.begin();
    nextIt++;
    for (int i=0; i+1<(int)rowData.size(); i++)
    {
        if (it->nev == nextIt->nev)
            return it->nev;
        it++;
        nextIt++;
    }
    return 0;
}


DbRecord DbModel::getOneRowData(int rowIndex)
{
    if (  rowIndex<(int)rowData.size() )
    {
        list<DbRecord>::iterator it = rowData.begin();
        advance(it, rowIndex);
        return *it;
    }
    DbRecord tmp;
    tmp.nev = "";
    return tmp;
}

Qt::ItemFlags DbModel::flags(const QModelIndex &index) const
{
    return  Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool DbModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        DbRecord oneRowData;
        if ((int)rowData.size()<=index.row())
        {//update the table data before we load other row data
            rowData.push_back(oneRowData);
        }
        oneRowData = getOneRowData(index.row());
        //save value from editor to member m_gridData
        switch (index.column()) {
        case 0:
            oneRowData.nev = value.toString();
            //add default date to current date
            oneRowData.date = QDate::currentDate().toString("yyyy-MM-dd");
            break;
        case 1:
            oneRowData.reszleg = value.toString();
            break;
        case 2:
            oneRowData.muszak = value.toString();
            break;
        case 3:
            if (value.toString()!="")
                oneRowData.date = value.toString();
            break;
        case 4:
            oneRowData.email = value.toString();
            break;

        }
        updateTableData(index.row(), oneRowData);

        //emit editCompleted(result);
        return true;
    }
    return false;
}
