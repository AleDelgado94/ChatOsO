#include "sslserver.h"

#include <QSslSocket>
#include <QSslCertificate>
#include <QSslKey>

SslServer::SslServer(QString dir, quint16 port, QSqlDatabase *db, QObject *parent):
    QTcpServer(parent),
    db(db),
    dir_(dir),
    port_(port)
{

}


void SslServer::incomingConnection(qintptr socketDescriptor){

    qDebug() << "Nueva Conexión";
    QSslSocket* socket = new QSslSocket();

    if(socket->setSocketDescriptor(socketDescriptor)){
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), socket, SLOT(ignoreSslErrors()));
        //connect(socket, SIGNAL(encrypted()), this, SLOT(ready()));
        socket->setProtocol(QSsl::TlsV1_1);
        socket->setPrivateKey("../Certificados/server.key");
        socket->setLocalCertificate("../Certificados/server.crt");
        socket->ignoreSslErrors();
        //addPendingConnection(socket);

        Client *client = new Client(socket, db, this);
        //list_clients.insert(client, socket->socketDescriptor());
        socket->startServerEncryption();
    }
    else{
        delete socket;
    }

}


