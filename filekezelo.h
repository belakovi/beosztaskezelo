#ifndef FILEKEZELO_H
#define FILEKEZELO_H
#include <stdlib.h>
#include <QFile>

#define EMPTYFILE 0

using namespace std;

class filekezelo
{
public:
    //filekezelo();
    int filenyitas(string filenev);
    QString fileolvasas();
    void closefile();
    void overwritefile(QString adat);

private:
    QFile *file;

};



#endif // FILEKEZELO_H
