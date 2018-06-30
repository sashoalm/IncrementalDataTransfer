#-------------------------------------------------
#
# Project created by QtCreator 2018-06-17T15:51:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataTransfer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    incrementaldatatransfer.cpp \
    mockiodevice.cpp

HEADERS  += mainwindow.h \
    incrementaldatatransfer.h \
    mockiodevice.h

FORMS    += mainwindow.ui

CONFIG += c++11

