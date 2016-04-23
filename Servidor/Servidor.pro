QT += core
QT -= gui

CONFIG += c++11

TARGET = Servidor
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=

DISTFILES += \
    protobuf.pri \
    protomessage.proto

PROTOS = protomessage.proto
include(protobuf.pri)

