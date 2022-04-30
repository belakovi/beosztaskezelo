#ifndef NAPMODEL_H
#define NAPMODEL_H
#include <QAbstractTableModel>
#include <list>
#include <QDate>

using namespace std;

#define MAXHET 6
#define MAXNAP 7
#define MUNKANAP  "M"
#define SZABADNAP "F"
#define LIST_BY_NAME 1
#define LIST_BY_RESZLEG 2

const QStringList columnHeaderNap = {"Dátum", "Státusz"};
const QStringList napStatus = {"Munkanap", "Szabad"};

struct DbNapRecord
{
    QDate   datum;
    QString status;

    bool operator <(const DbNapRecord & dbrecordObj) const
    {
        return datum < dbrecordObj.datum;
    }
} ;

class napmodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    napmodel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setRowCount(int row);
    void clearTable(void);
    void addTableData(QStringList data);
    void updateTableData(int rowIndex, DbNapRecord dbData);
    void deleteTableData(int rowIndex);
    bool getOneRowData(int rowIndex, DbNapRecord &record);
    bool checkSameDate(QDate &name);
    void addOneRowToTable();
    int getEmptyID();
    bool compare_date (DbNapRecord& first, DbNapRecord& second);

private:
    int numberOfRow = 0;
    list<DbNapRecord> rowNapData;
    int currentOrder = LIST_BY_NAME;

signals:
    void editCompleted(const QString &);
};

#endif // NAPMODEL_H
