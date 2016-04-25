#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtSql>
#include "protomessage.pb.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QTcpSocket* tcpSocket, QObject *parent = 0);

signals:

public slots:
    void readyRead();

private:
    QTcpSocket *tcpSocket_;
};

#endif // CLIENT_H
