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

#include "login.h"
#include "protomessage.pb.h"

class My_Socket_Cliente : public QObject
{
    Q_OBJECT
private:
    QSslSocket* sslSocket;//socket que vamos a utilizar
    QString username;
    QString password;
    QString dir_ip;

public:
    explicit My_Socket_Cliente(QString dir_server, quint16 port_server, QString user_name, QString passwd, QObject *parent = 0);


signals:

public slots:
    void ready();
    void readyRead();
};

#endif // MY_SOCKET_CLIENTE_H
