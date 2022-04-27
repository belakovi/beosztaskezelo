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

bool DbModel::compare_name (DbRecord& first, DbRecord& second)
{
    return (first.nev > second.nev);
}

void DbModel::addTableData(QStringList dbData)
{
    DbRecord data;
    data.id      = dbData.at(0).toInt();
    data.nev     = dbData.at(1);
    data.reszleg = dbData.at(2);
    data.email   = dbData.at(3);
    rowData.push_back(data);

    rowData.sort();
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
    switch (role)
    {
    case Qt::DisplayRole:
        int row = index.row();
        if (row>=(int)rowData.size())
            return QVariant();
        auto it =  rowData.begin();
        advance(it, row);
        switch (index.column())
        {
        case 0:
            return it->nev;
        case 1:
            return it->reszleg;
        case 2:
            return it->email;
        }
    }

    return QVariant();
}

bool compare_nocase (const DbRecord& first, const DbRecord& second)
{
    return (first.nev<second.nev);
}

bool DbModel::checkSameName(QString &name)
{
    rowData.sort(compare_nocase);
    list<DbRecord>::iterator it = rowData.begin();
    list<DbRecord>::iterator nextIt = rowData.begin();
    nextIt++;
    for (int i=0; i+1<(int)rowData.size(); i++)
    {
        if (it->nev == nextIt->nev && it->reszleg==nextIt->reszleg)
        {
            name = it->nev;
            return true;
        }
        it++;
        nextIt++;
    }
    return false;
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

int DbModel::getEmptyID()
{
    list<DbRecord>::iterator it;

    //search for empty place
    for (int i=0; i<INT_MAX; i++)
    {        
        for (it=rowData.begin(); it != rowData.end(); ++it)
        {
            if (it->id==i)
                break;
        }
        if(it == rowData.end())
            return i;
    }
    return INT_MAX;
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
            if (oneRowData.nev == "")
            {
                auto it =  rowData.begin();
                advance(it, index.row());
                rowData.erase(it);
                return true;
            }

            //add unique id to the name, later check if no more empty spot then error
            oneRowData.id = getEmptyID();
            break;
        case 1:
            oneRowData.reszleg = value.toString();
            break;
        case 2:
            oneRowData.email = value.toString();
            break;

        }
        updateTableData(index.row(), oneRowData);

        //emit editCompleted(result);
        return true;
    }
    return false;
}
