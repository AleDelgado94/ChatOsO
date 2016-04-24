#ifndef MYUDP_H
#define MYUDP_H

#include <QObject>
#include <QUdpSocket>
#include <iostream>
#include <QtSql>

#include "protomessage.pb.h"

class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QString dirServer, quint16 portServer ,QObject *parent = 0);
    void send(Message packet);

signals:

public slots:
    void readyRead();

public:
    QUdpSocket *socket;
};

#endif // MYUDP_H
