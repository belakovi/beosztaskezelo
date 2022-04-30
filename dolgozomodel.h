#ifndef DOLGOZOMODEL_H
#define DOLGOZOMODEL_H

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

const QStringList columnHeaderDolgozo = {"Név", "Részleg", "Email"};

struct DbDolgozoRecord
{
    int     id;
    QString nev;
    QString reszleg;
    QString email;

    bool operator <(const DbDolgozoRecord & dbrecordObj) const
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

class dolgozoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    dolgozoModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setRowCount(int row);
    void clearTable(void);
    void addTableDolgozoData(QStringList data);
    void updateTableData(int rowIndex, DbDolgozoRecord dbData);
    void deleteTableData(int rowIndex);
    DbDolgozoRecord getOneRowData(int rowIndex);
    bool checkSameName(QString &name);
    void addOneRowToTable();
    int getEmptyID();
    bool compare_name (DbDolgozoRecord& first, DbDolgozoRecord& second);

private:
    int numberOfRow = 0;
    list<DbDolgozoRecord> rowDolgozoData;
    int currentOrder = LIST_BY_NAME;

signals:
    void editCompleted(const QString &);
};

#endif // MYMODEL_H
