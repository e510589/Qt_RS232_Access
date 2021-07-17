#-------------------------------------------------
#
# Project created by QtCreator 2020-02-12T13:24:15
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = link_cell_test
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    link_data.cpp

HEADERS  += widget.h \
    link_data.h

FORMS    += widget.ui
