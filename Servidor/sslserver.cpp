#include "sslserver.h"

#include <QSslSocket>
#include <QSslCertificate>
#include <QSslKey>

SslServer::SslServer(QString dir, quint16 port, QString rutaCert, QString rutaKey, QSqlDatabase *db, QObject *parent):
    QTcpServer(parent),
    rutaCert_(rutaCert),
    rutaKey_(rutaKey),
    db(db)
{
}



void SslServer::incomingConnection(quintptr socketDescriptor){

    QSslSocket* socket = new QSslSocket();



    if(socket->setSocketDescriptor(socketDescriptor)){
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), socket, SLOT(ignoreSslErrors()));
        socket->setProtocol(QSsl::AnyProtocol);
        socket->ignoreSslErrors();
        socket->setPrivateKey("../Certificados/server.key");
        socket->setPeerVerifyMode(QSslSocket::QueryPeer);
       // socket->addCaCertificate("../Certificados/server.crt");
        socket->setLocalCertificate("../Certificados/server.crt");
        addPendingConnection(socket);
        //clients.append(socket);
       // connect(socket, SIGNAL(encrypted()), this, SLOT(ready()));
        //socket->setProtocol(QSsl::TlsV1_2OrLater);
        socket->startServerEncryption();
    }
    else{
        delete socket;
        //clients.removeLast();
    }

}


void SslServer::ready()
{/*
    //Se ejecuta cuando se encripta la comunicación
   // QByteArray first_massage = socket->readAll();
    QString message(first_massage);
    Log packet;
    packet.ParseFromString(message.toStdString());

    QString username = QString::fromStdString(packet.name_user());
    QString pass = QString::fromStdString(packet.password());
    QString ip = QString::fromStdString(packet.ip_user());
    uint32_t port = packet.port_user();

    QSqlDatabase *db = new QSqlDatabase("QSQLITE");
    db->setDatabaseName("./database.sqlite");
    QSqlQuery query(*db);

    std::string login = "SELECT * FROM LOGIN WHERE usuario = '" + username.toStdString() + "' AND password = '" + pass.toStdString() + "';";
    query.exec(QString::fromStdString(login));

    while(query.next()){
        if(query.value("usuario").toString() == username){
            //SIGNIFICA QUE EL USUARIO ESTÁ REGISTRADO
            socket->connectToHostEncrypted(ip,quint16(port));
            //TODO: enviar avatares
            socket->write("OK");
        }
    }
*/

}



void SslServer::readyRead()
{

}

