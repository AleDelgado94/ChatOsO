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
   QByteArray Package = ;
}

void My_Socket_Cliente::readyRead()//para cunado el servidor me reenvie los mensajes de otros user
    QByteArray data_server = sslSocket->readAll();

}


