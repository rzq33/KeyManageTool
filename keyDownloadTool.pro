#-------------------------------------------------
#
# Project created by QtCreator 2019-12-11T13:55:00
#
#-------------------------------------------------

QT       += core gui

QT       += serialport #����QT����ͨ�����

CONFIG += c++11  #����C++11��׼

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets #�Ͱ汾���¼���

TARGET = keyDownloadTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tool.cpp

HEADERS  += mainwindow.h \
    tool.h

FORMS    += mainwindow.ui

RC_ICONS = keypad.ico
