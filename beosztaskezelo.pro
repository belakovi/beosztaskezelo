QT += core gui sql
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adatkezelo.cpp \
    comboboxitemdelegate.cpp \
    dbmanager.cpp \
    dolgozomodel.cpp \
    general.cpp \
    main.cpp \
    mainwindow.cpp \
    munkanapvalt.cpp \
    mymodel.cpp \
    napmodel.cpp

HEADERS += \
    adatkezelo.h \
    comboboxitemdelegate.h \
    dbmanager.h \
    dolgozomodel.h \
    general.h \
    mainwindow.h \
    munkanapvalt.h \
    mymodel.h \
    napmodel.h

FORMS += \
    adatkezelo.ui \
    general.ui \
    mainwindow.ui \
    munkanapvalt.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    hatter.qrc \
    foldericon.png

