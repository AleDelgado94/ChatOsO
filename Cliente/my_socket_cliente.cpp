#include "my_socket_cliente.h"

My_Socket_Cliente::My_Socket_Cliente(QString dir_server, quint16 port_server, QString dir_socket_client,QObject *parent) :

    QObject(parent)
{
    sslSocket = new QSslSocket(this);//creamos el socket que vamos a usar
    sslSocket->connectToHostEncrypted(dir_server, port_server);
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void My_Socket_Cliente::ready()//solo para enviar mensajes al servidor(logearme, esperar mensajes,...)
{
    QByteArray data = sslSocket->readAll();
    //TODO:procesar datos (deserializar)
    //TODO2:preparar respuesta "dataOUT"
    //sslSocket->write(/*TODO2*/);
}

void My_Socket_Cliente::readyRead()//para enviarle mensajes al servidor ya cuando escribimos
{

}


