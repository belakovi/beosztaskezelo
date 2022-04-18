#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>

class ComboBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboBoxItemDelegate(QObject *parent = nullptr, QStringList items = {""});
    ~ComboBoxItemDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
    QStringList comboTextItems;

private slots:
    void onComboChanged(const QString & text);
};

#endif // COMBOBOXITEMDELEGATE_H
