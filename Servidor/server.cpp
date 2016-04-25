#include "server.h"

Server::Server(QString dir, quint16 port, QObject *parent) :
    QObject(parent)
{
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress(dir), port);

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::newConnection()
{
    while(tcpServer->hasPendingConnections()){
        QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
        Client *client = new Client(clientConnection);
        clients.append(client);
    }
}
