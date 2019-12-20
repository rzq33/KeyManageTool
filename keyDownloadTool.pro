#-------------------------------------------------
#
# Project created by QtCreator 2019-12-11T13:55:00
#
#-------------------------------------------------

QT       += core gui

QT       += serialport #启用QT串口通信类库

CONFIG += c++11  #兼容C++11标准

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets #低版本向下兼容

TARGET = keyDownloadTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tool.cpp

HEADERS  += mainwindow.h \
    tool.h

FORMS    += mainwindow.ui

RC_ICONS = keypad.ico
