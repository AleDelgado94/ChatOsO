QT += core
QT += gui
QT += network
QT += sql
QT += multimedia multimediawidgets

PKGCONFIG += openssl

CONFIG += c++11

TARGET = ServidorChatOsO
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    client.cpp \
    sslserver.cpp \
    server.cpp

DISTFILES += \
    protobuf.pri \
    protomessage.proto \
    Images/haribo.jpg

PROTOS = protomessage.proto
include(protobuf.pri)

HEADERS += \
    client.h \
    sslserver.h \
    server.h

unix {          # Esta configuración específica de Linux y UNIX
    # Variables
    #
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    MOC_DIR= ./moc
    OBJECTS_DIR=./object

    BINDIR  = $$PREFIX/bin
    DATADIR = $$PREFIX/share
    CONFDIR = /etc
    SCRIPT = /etc/init.d

isEmpty(VARDIR) {
        VARDIR  = /var/lib/$${TARGET}
    }

    DEFINES += APP_DATADIR=\\\"$$DATADIR\\\"
    DEFINES += APP_VARDIR=\\\"$$VARDIR\\\"
    DEFINES += APP_CONFFILE=\\\"$$CONFDIR/$${TARGET}.ini\\\"
    DEFINES += APP_CLAVCERT=\\\$$CONFDIR/$${TARGET}\\\"

    # Install
    #
    INSTALLS += target config vardir script

    ## Instalar ejecutable
    target.path = $$BINDIR

    ## Instalar archivo de configuración
    config.path = $$CONFDIR
    config.files += $${TARGET}.ini
    config.extra = mkdir -p $$CONFDIR/$${TARGET} && mkdir -p $$CONFDIR/$${TARGET}/Certificados && cp ../Certificados/* $$CONFDIR/$${TARGET}/Certificados

    ## Crear directorio de archivos variables
    vardir.path = $$VARDIR
    vardir.extra =   mkdir -p $$VARDIR/clientes && mkdir -p $$VARDIR/Images && mkdir -p $$VARDIR/database && cp ../Servidor/database.db $$VARDIR/database  && mkdir -p $$VARDIR/estadisticas
    vardir.commands = true

    ## Script
    script.path = $$SCRIPT
    script.files += ../$${TARGET}.sh
    script.extra = cp ../$${TARGET}.sh /etc/init.d && chmod ug+x /etc/init.d/$${TARGET}.sh && update-rc.d ServidorChatOsO defaults
}
