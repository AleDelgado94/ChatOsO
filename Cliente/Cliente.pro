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

PROTOS = protomessage.proto
include(protobuf.pri)

unix {
    # Variables
    #
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    CONFDIR = /etc

    isEmpty(VARDIR) {
        VARDIR = /var/lib/$${TARGET}
    }

    DEFINES += APP_DATADIR="$$DATADIR"
    DEFINES += APP_VARDIR='"$$VARDIR"'
    DEFINES += APP_CONFFILE="$$CONFDIR/$${TARGET}.ini"

    # Install
    #
    INSTALLS += target config desktop icon32 vardir
        ## Instalar ejecutable
        target.path = $$BINDIR

        ## Instalar archivo de configuración
        config.path = $$CONFDIR
        config.files += $${TARGET}.ini

        ## Instalar acceso directo en el menú del escritorio
        desktop.path = $$DATADIR/applications
        desktop.files += $${TARGET}.desktop

        ## Instalar icono de aplicación
        icon32.path = $$DATADIR/icons/hicolor/32x32/apps
        icon32.files += ./data/32x32/$${TARGET}.png

    ## Crear directorio de archivos variables
    vardir.path = $$VARDIR
    vardir.commands = true
}

