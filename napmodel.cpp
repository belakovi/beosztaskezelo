#include "napmodel.h"
#include <QMessageBox>

napmodel::napmodel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void napmodel::setRowCount(int row)
{
    numberOfRow = row;
}

int napmodel::rowCount(const QModelIndex & /*parent*/) const
{
   return numberOfRow;
}

int napmodel::columnCount(const QModelIndex & /*parent*/) const
{
    return columnHeaderNap.size();
}

QVariant napmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return columnHeaderNap.at(section);
    }

    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        return section;
    }
    return QVariant();
}

bool napmodel::compare_date(DbNapRecord &first, DbNapRecord &second)
{
    return (first.datum > second.datum);
}

void napmodel::addTableData(QStringList dbData)
{
    DbNapRecord data;
    data.datum  = QDate::fromString(dbData.at(0),"yyyy-MM-dd");
    if (dbData.at(1) == MUNKANAP)
        data.status = napStatus[0];
    else
        data.status = napStatus[1];

    rowNapData.push_back(data);

    rowNapData.sort();
}

void napmodel::updateTableData(int rowIndex, DbNapRecord dbData)
{
    if(rowIndex<0 || rowIndex>(int)rowNapData.size()) //if index points to invalid place then don't update
        return;
    list<DbNapRecord>::iterator it = rowNapData.begin();
    advance(it, rowIndex);
    it->datum = dbData.datum;
    it->status = dbData.status;
}

void napmodel::clearTable()
{
    beginResetModel();
    numberOfRow = 0;
    rowNapData.clear();
    endResetModel();
}

void napmodel::addOneRowToTable()
{
    beginResetModel();
    numberOfRow += 1;
    endResetModel();
}

QVariant napmodel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        int row = index.row();
        if (row>=(int)rowNapData.size())
            return QVariant();
        auto it =  rowNapData.begin();
        advance(it, row);
        switch (index.column())
        {
        case 0:
            return it->datum.toString("yyyy-MM-dd");
        case 1:
            return it->status;
        }
    }

    return QVariant();
}

bool compare_datum (const DbNapRecord& first, const DbNapRecord& second)
{
    return (first.datum<second.datum);
}

bool napmodel::checkSameDate(QDate &actDate)
{
    rowNapData.sort(compare_datum);
    list<DbNapRecord>::iterator it = rowNapData.begin();
    list<DbNapRecord>::iterator nextIt = rowNapData.begin();
    nextIt++;
    for (int i=0; i+1<(int)rowNapData.size(); i++)
    {
        if (it->datum == nextIt->datum)
        {
            actDate = it->datum;
            return true;
        }
        it++;
        nextIt++;
    }
    return false;
}

bool napmodel::getOneRowData(int rowIndex, DbNapRecord &record)
{
    if (  rowIndex<(int)rowNapData.size() )
    {
        list<DbNapRecord>::iterator it = rowNapData.begin();
        advance(it, rowIndex);
        record = *it;
        return true;
    }
    return false;
}

Qt::ItemFlags napmodel::flags(const QModelIndex &index) const
{
    return  Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool napmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        DbNapRecord oneRowData;
        if ((int)rowNapData.size()<=index.row())
        {//update the table data before we load other row data
            for (int i=(int)rowNapData.size(); i<=index.row(); i++)
                rowNapData.push_back(oneRowData);
        }

        getOneRowData(index.row(), oneRowData);
        switch (index.column()) {
        case 0:
            if(value.toString()!="")
            {
                oneRowData.datum = QDate::fromString(value.toString(), "yyyy-MM-dd");
                if(!oneRowData.datum.isValid())
                {
                    QMessageBox msgBox;
                    QString hiba("Rossz dátum, ellenörizze a formátumot vagy az értéket (yyyy-mm-dd): %1");
                    msgBox.setText(hiba.arg(value.toString()));
                    msgBox.exec();
                    return false;
                }
                updateTableData(index.row(), oneRowData);
            }
            break;
        case 1:
            if(value.toString()!="")
            {
                oneRowData.status = value.toString();
                updateTableData(index.row(), oneRowData);
            }
            break;
        }

        //emit editCompleted(result);
        return true;
    }
    return false;
}

