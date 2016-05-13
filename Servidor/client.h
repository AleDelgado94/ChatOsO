#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtSql>
#include <QSslSocket>
#include <QSslError>
#include <QHostAddress>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QPaintDevice>
#include <QDataStream>

#include "protomessage.pb.h"


static QMap<QString, QSslSocket*> list_clients;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QSslSocket* sslSocket, QSqlDatabase *db , QObject *parent = 0);
    Message deserializar();

signals:

public slots:
    void readyRead();
    void error();
    //void firstConnection();

private:


    //QTcpSocket *tcpSocket_;
    QSslSocket *sslSocket_;
    quint32 tamPacket;
    QSqlDatabase* db;
    quintptr socketDescriptor;
    Message message;

};

#endif // CLIENT_H
