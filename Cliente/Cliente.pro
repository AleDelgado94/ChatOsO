#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T12:01:38
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cliente
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    ventanaprincipal.cpp

HEADERS  += login.h \
    ventanaprincipal.h

FORMS    += login.ui \
    ventanaprincipal.ui
