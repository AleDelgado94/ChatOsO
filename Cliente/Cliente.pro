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
    perfil.cpp \
    ventanaprincipal.cpp \
    configure.cpp \
    my_socket_cliente.cpp

HEADERS  += login.h \
    perfil.h \
    ventanaprincipal.h \
    configure.h \
    my_socket_cliente.h

FORMS    += login.ui \
    ventanaprincipal.ui \
    configure.ui \
    perfil.ui

PROTOS = protomessage.proto
include(protobuf.pri)
