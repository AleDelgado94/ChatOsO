#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtSql>
#include <QSslSocket>
#include <QSslError>


#include "protomessage.pb.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QSslSocket* sslSocket, QSqlDatabase *db , QObject *parent = 0);


signals:

public slots:
    void readyRead();
    void error();

private:
    //QTcpSocket *tcpSocket_;
    QSslSocket *sslSocket_;
    quint32 tamPacket;
    QSqlDatabase* db;
    quintptr socketDescriptor;
    Message message;
    Log log;

};

#endif // CLIENT_H
