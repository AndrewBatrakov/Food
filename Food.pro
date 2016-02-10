#-------------------------------------------------
#
# Project created by QtCreator 2016-02-09T17:09:59
#
#-------------------------------------------------

QT       += core gui\
            sql\
            network\

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Food
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

RESOURCES += \
    resourse.qrc

RC_FILE = food.rc

DISTFILES +=

include(Service.pri)
include(Document.pri)
include(Reference.pri)
include(Report.pri)
include(SQLTable.pri)
