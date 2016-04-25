#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QtSql>

#include "client.h"




class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QString dir, quint16 port, QObject *parent = 0);

signals:

public slots:
    void newConnection();

private:
    QTcpServer* tcpServer;
    QList<Client*> clients;


};


#endif // SERVER_H
