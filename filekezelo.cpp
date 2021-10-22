#include "filekezelo.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

/*filekezelo()
{
    else
    {
        QTextStream in(adatbazisFile);
        QString text = in.readAll();
        QStringList cells = text.split("#");
        QModelIndex index;

        for (int row = 0; row < model->rowCount() && cells.size()-1 > row*model->columnCount(); row++)
        {
            for (int col = 0; col < model->columnCount(); col++)
            {
                if (cells.at(row*model->columnCount()+col) != "")
                {
                    index = model->index(row, col);
                    model->setData(index, cells.at(row*model->columnCount()+col));
                }
            }
        }
    }
}*/
int filekezelo::filenyitas(string filenev)
{

    file= new QFile(QString::fromStdString(filenev));
    if (!file->open(QFile::ReadWrite | QFile::Text) || file->size() == 0)
    {
        return EMPTYFILE;

    }
    return file->size();
}

QString filekezelo::fileolvasas()
{
    QTextStream in(file);
    QString text = in.readAll();
    return text;
}

void filekezelo::overwritefile(QString adat)
{
    file->resize(0);
    QTextStream stream(file);
    stream << adat;
}

void filekezelo::closefile()
{
    file->close();
}
