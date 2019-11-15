#-------------------------------------------------
#
# Project created by QtCreator 2017-04-28T12:31:56
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTreeModel
TEMPLATE = app


SOURCES += main.cpp\
    modelbuilder.cpp \
    widget.cpp

HEADERS  += \
    modelbuilder.h \
    widget.h

FORMS    += widget.ui

RESOURCES += \
    simpletreemodel.qrc

OTHER_FILES +=

DISTFILES +=
