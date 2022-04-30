#include "dolgozomodel.h"
#include <QDate>

dolgozoModel::dolgozoModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void dolgozoModel::setRowCount(int row)
{
    numberOfRow = row;
}

int dolgozoModel::rowCount(const QModelIndex & /*parent*/) const
{
   return numberOfRow;
}

int dolgozoModel::columnCount(const QModelIndex & /*parent*/) const
{
    return columnHeaderDolgozo.size();
}

QVariant dolgozoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return columnHeaderDolgozo.at(section);
    }

    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return section;
    }
    return QVariant();
}

bool dolgozoModel::compare_name (DbDolgozoRecord& first, DbDolgozoRecord& second)
{
    return (first.nev > second.nev);
}

void dolgozoModel::addTableDolgozoData(QStringList dbData)
{
    DbDolgozoRecord data;
    data.id      = dbData.at(0).toInt();
    data.nev     = dbData.at(1);
    data.reszleg = dbData.at(2);
    data.email   = dbData.at(3);
    rowDolgozoData.push_back(data);

    rowDolgozoData.sort();
}

void dolgozoModel::updateTableData(int rowIndex, DbDolgozoRecord dbData)
{
    if(rowIndex<0 || rowIndex>(int)rowDolgozoData.size()) //if index points to invalid place then don't update
        return;
    auto it =  rowDolgozoData.begin();
    advance(it, rowIndex);
    *it = dbData;
}

void dolgozoModel::clearTable()
{
    beginResetModel();
    numberOfRow = 0;
    rowDolgozoData.clear();
    endResetModel();
}

void dolgozoModel::addOneRowToTable()
{
    beginResetModel();
    numberOfRow += 1;
    endResetModel();
}

QVariant dolgozoModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        int row = index.row();
        if (row>=(int)rowDolgozoData.size())
            return QVariant();
        auto it =  rowDolgozoData.begin();
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

bool compare_nocase (const DbDolgozoRecord& first, const DbDolgozoRecord& second)
{
    return (first.nev<second.nev);
}

bool dolgozoModel::checkSameName(QString &name)
{
    rowDolgozoData.sort(compare_nocase);
    list<DbDolgozoRecord>::iterator it = rowDolgozoData.begin();
    list<DbDolgozoRecord>::iterator nextIt = rowDolgozoData.begin();
    nextIt++;
    for (int i=0; i+1<(int)rowDolgozoData.size(); i++)
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

DbDolgozoRecord dolgozoModel::getOneRowData(int rowIndex)
{
    if (  rowIndex<(int)rowDolgozoData.size() )
    {
        list<DbDolgozoRecord>::iterator it = rowDolgozoData.begin();
        advance(it, rowIndex);
        return *it;
    }
    DbDolgozoRecord tmp;
    tmp.nev = "";
    return tmp;
}

Qt::ItemFlags dolgozoModel::flags(const QModelIndex &index) const
{
    return  Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

int dolgozoModel::getEmptyID()
{
    list<DbDolgozoRecord>::iterator it;

    //search for empty place
    for (int i=0; i<INT_MAX; i++)
    {        
        for (it=rowDolgozoData.begin(); it != rowDolgozoData.end(); ++it)
        {
            if (it->id==i)
                break;
        }
        if(it == rowDolgozoData.end())
            return i;
    }
    return INT_MAX;
}

bool dolgozoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        DbDolgozoRecord oneRowData;
        if ((int)rowDolgozoData.size()<=index.row())
        {//update the table data before we load other row data
            rowDolgozoData.push_back(oneRowData);
        }
        oneRowData = getOneRowData(index.row());
        //save value from editor to member m_gridData
        switch (index.column()) {
        case 0:
            oneRowData.nev = value.toString();
            if (oneRowData.nev == "")
            {
                auto it =  rowDolgozoData.begin();
                advance(it, index.row());
                rowDolgozoData.erase(it);
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
