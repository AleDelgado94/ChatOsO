#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlDriver>
#include "client.h"
#include "sslserver.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QString dir, quint16 port, QObject *parent = 0);
    void run();

signals:

public slots:


private:
    SslServer *servidor;
    QSqlDatabase *db;
    QList<Client*> clients;
    QString dir_;
    quint16 port_;


};

#endif // SERVER_H
