#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T12:01:38
#
#-------------------------------------------------

QT       += core sql
QT += gui
QT       += network
QT       += multimedia multimediawidgets
QT += widgets


PKGCONFIG += openssl


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatOsO
TEMPLATE = app


SOURCES += main.cpp\
    ventanaprincipal.cpp \
    configure.cpp \
    my_socket_cliente.cpp \
    chatwindows.cpp \
    loginuser.cpp \
    perfilusers.cpp \
    ventanacamera.cpp


HEADERS  += \
    ventanaprincipal.h \
    configure.h \
    my_socket_cliente.h \
    chatwindows.h \
    loginuser.h \
    perfilusers.h \
    capturebuffer.h \
    ventanacamera.h

FORMS    += \
    ventanaprincipal.ui \
    configure.ui \
    chatwindows.ui \
    loginuser.ui \
    perfilusers.ui \
    ventanacamera.ui

PROTOS = protomessage.proto
include(protobuf.pri)


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




    isEmpty(VARDIR) {
        VARDIR  = /var/lib/$${TARGET}
    }

    DEFINES += APP_DATADIR=\\\"$$DATADIR\\\"
    DEFINES += APP_VARDIR=\\\"$$VARDIR\\\"
    DEFINES += APP_CONFFILE=\\\"$$CONFDIR/$${TARGET}.ini\\\"

    # Install
    #
    INSTALLS += target config icon32 desktop vardir

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
    icon32.files += ../data/32x32/$${TARGET}.png


    ## Crear directorio de archivos variables
    vardir.path = $$VARDIR

    vardir.extra = mkdir -p ~/.local/ChatOsO/ && mkdir -p ~/.local/ChatOsO/Images && chmod 777 ~/.local/ChatOsO/Images && mkdir -p $$DATADIR/ChatOsO/Usuarios && chmod 777 $$DATADIR/ChatOsO/Usuarios
    vardir.commands = true

}


