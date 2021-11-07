#include "comboboxitemdelegate.h"
#include <QComboBox>

ComboBoxItemDelegate::ComboBoxItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

ComboBoxItemDelegate::~ComboBoxItemDelegate()
{
}

QWidget *ComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Create the combobox and populate it
    QComboBox *cb = new QComboBox(parent);
    const int row = index.row();
    cb->addItem(QString("Nappali").arg(row));
    cb->addItem(QString("Ejszakai").arg(row));
    return cb;
}

void ComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    // get the index of the text in the combobox that matches the current value of the item
    const QString currentText = index.data(Qt::EditRole).toString();
    const int cbIndex = cb->findText(currentText);
    // if it is valid, adjust the combobox
    if (cbIndex >= 0)
       cb->setCurrentIndex(cbIndex);
}

void ComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->currentText(), Qt::EditRole);
}

void ComboBoxItemDelegate::onComboChanged(const QString & text)
{
    qDebug() << "onComboChanged " << text;
    QComboBox * comboBox = dynamic_cast<QComboBox *>(sender());

    if(comboBox)
     {
         int column = comboBox->objectName().toInt();  // retrieve column number
         qDebug() << "Dropdown: cols " << column << " : " << comboBox->currentText() << " index " << comboBox->currentIndex();
     }
}
