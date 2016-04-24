QT += core
QT -= gui
QT += network
QT += sql

CONFIG += c++11

TARGET = Servidor
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    myudp.cpp

DISTFILES += \
    protobuf.pri \
    protomessage.proto

PROTOS = protomessage.proto
include(protobuf.pri)

HEADERS += \
    myudp.h

