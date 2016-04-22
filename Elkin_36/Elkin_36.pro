#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T01:40:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Elkin_36
TEMPLATE = app


CONFIG +=c++11
SOURCES += main.cpp\
        mainwindow.cpp\
        qcustomplot.cpp\
        FFTA/utilities.cpp\
        FFTA/ffta.cpp

HEADERS  += mainwindow.h\
	qcustomplot.h\
	FFTA/utilities.h\
	FFTA/ffta.h
#include "FFTA/functions.h

FORMS    += mainwindow.ui
