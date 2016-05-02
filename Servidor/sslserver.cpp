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



void SslServer::incomingConnection(quintptr socketDescriptor){

    QSslSocket* socket = new QSslSocket();

    qDebug() << "CHIVATO";


    if(socket->setSocketDescriptor(socketDescriptor)){
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), socket, SLOT(ignoreSslErrors()));
        socket->setProtocol(QSsl::AnyProtocol);
        socket->setPrivateKey("../Certificados/server.key");
        socket->setLocalCertificate("../Certificados/server.crt");
        socket->ignoreSslErrors();
        addPendingConnection(socket);
        socket->startServerEncryption();
        Client *client = new Client(socket, db, this);
        list_clients.insert(client, socket->socketDescriptor());
    }
    else{
        delete socket;
    }

}
