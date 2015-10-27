#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T14:45:58
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        dialog.cpp \
    handler.cpp \
    server.cpp \
    room.cpp \
    member.cpp

HEADERS  += dialog.h \
    handler.h \
    message.h \
    server.h \
    room.h \
    member.h

FORMS    += dialog.ui
