#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QSslKey>
#include <QList>
#include <QtSql>
#include "client.h"

#include "protomessage.pb.h"

class SslServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SslServer(QString dir, quint16 port, QString rutaCert, QString rutaKey, QSqlDatabase *db, QObject *parent = 0);


public slots:
    void ready();
    void readyRead();
protected:
    void incomingConnection(quintptr socketDescriptor);

private:


    QSqlDatabase* db;
    QString rutaCert_;
    QString rutaKey_;


};

#endif // SSLSERVER_H
