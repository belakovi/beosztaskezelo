#ifndef DBMODEL_H
#define DBMODEL_H

#include <QAbstractTableModel>
#include <list>

using namespace std;

#define MAXHET 6
#define MAXNAP 7

typedef struct {
    QString nev;
    QString reszleg;
    QString muszak;
    QString date;
    QString email;
} DbRecord;

typedef struct {
    QString nev;
    QString hetiBeosztas;
} DbBeosztas;

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
    DbRecord getOneRowData(int rowIndex);
    QString checkSameName();
    void addOneRowToTable();

private:
    int numberOfRow = 0;
    list<DbRecord> rowData;
    const QStringList columnHeader = {"Név", "Részleg", "Muszak", "Munkakezdés", "Email"};

signals:
    void editCompleted(const QString &);
};

#endif // MYMODEL_H
