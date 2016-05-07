#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T12:01:38
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cliente
TEMPLATE = app


SOURCES += main.cpp\
    ventanaprincipal.cpp \
    configure.cpp \
    my_socket_cliente.cpp \
    chatwindows.cpp \
    loginuser.cpp \
    perfiluser.cpp


HEADERS  += \
    ventanaprincipal.h \
    configure.h \
    my_socket_cliente.h \
    chatwindows.h \
    loginuser.h \
    perfiluser.h

FORMS    += \
    ventanaprincipal.ui \
    configure.ui \
    chatwindows.ui \
    loginuser.ui \
    perfiluser.ui

PROTOS = protomessage.proto
include(protobuf.pri)

