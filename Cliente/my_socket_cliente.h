#ifndef MY_SOCKET_CLIENTE_H
#define MY_SOCKET_CLIENTE_H

#include <QObject>
#include <QSslSocket>
#include <QString>
#include <QBuffer>
#include <QHostAddress>
#include <QHostInfo>
#include <QList>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QAbstractSocket>

#include "protomessage.pb.h"

class My_Socket_Cliente : public QObject
{
    Q_OBJECT

public:
    explicit My_Socket_Cliente(QString dir_server, quint16 port_server, QString user_name, QString passwd, QObject *parent = 0);
    bool logeado;
    QSslSocket* sslSocket;//socket que vamos a utilizar
    QString username;
    QString password;
    QString ip_server;
    quint16 server_port;
    QHostAddress my_ip;
    quint16 my_port;

signals:

public slots:
    void ready();
    void readyRead();
};

#endif // MY_SOCKET_CLIENTE_H
