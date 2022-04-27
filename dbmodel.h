#ifndef DBMODEL_H
#define DBMODEL_H

#include <QAbstractTableModel>
#include <list>

using namespace std;

#define MAXHET 6
#define MAXNAP 7

struct DbRecord
{
    int     id;
    QString nev;
    QString reszleg;
    QString email;

    bool operator <(const DbRecord & dbrecordObj) const
    {
        return nev < dbrecordObj.nev;
    }
} ;

typedef struct
{
    int     id;
    QString ev;
} DbBeosztas;

typedef struct
{
    QString    nev;
    DbBeosztas beosztas;
} beosztas_t;

#define LIST_BY_NAME 1
#define LIST_BY_RESZLEG 2

class DbModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DbModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setRowCount(int row);
    void clearTable(void);
    void addTableData(QStringList data);
    void updateTableData(int rowIndex, DbRecord dbData);
    void deleteTableData(int rowIndex);
    DbRecord getOneRowData(int rowIndex);
    bool checkSameName(QString &name);
    void addOneRowToTable();
    int getEmptyID();
    bool compare_name (DbRecord& first, DbRecord& second);

private:
    int numberOfRow = 0;
    list<DbRecord> rowData;
    const QStringList columnHeader = {"Név", "Részleg", "Email"};
    int currentOrder = LIST_BY_NAME;

signals:
    void editCompleted(const QString &);
};

#endif // MYMODEL_H
