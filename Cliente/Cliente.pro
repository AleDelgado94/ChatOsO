#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T12:01:38
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network

PKGCONFIG += openssl


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cliente
TEMPLATE = app


SOURCES += main.cpp\
    perfil.cpp \
    ventanaprincipal.cpp \
    configure.cpp \
    my_socket_cliente.cpp \
    chatwindows.cpp \
    loginuser.cpp


HEADERS  += \
    perfil.h \
    ventanaprincipal.h \
    configure.h \
    my_socket_cliente.h \
    chatwindows.h \
    loginuser.h

FORMS    += \
    ventanaprincipal.ui \
    configure.ui \
    perfil.ui \
    chatwindows.ui \
    loginuser.ui

PROTOS = protomessage.proto
include(protobuf.pri)

