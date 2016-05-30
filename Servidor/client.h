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
#include <QImageReader>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QVector>

#include "protomessage.pb.h"

#define NumPruebas 10


static QMap<QString, QSslSocket*> list_clients;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QSslSocket* sslSocket, QSqlDatabase *db , QObject *parent = 0);
    Message deserializar();
    ~Client();

signals:

public slots:
    void readyRead();
    void error();
    void statistics();
    //void firstConnection();

private:


    //QTcpSocket *tcpSocket_;
    QSslSocket *sslSocket_;
    quint32 tamPacket;
    QSqlDatabase* db;
    quintptr socketDescriptor;
    Message message;
    QImage* avatar;
    QImageReader reader;
    QVector<int> tConsultasDB;
    QVector<int> tRecepcionPaquetes;
    QVector<int> tEnvioPaquetes;
    QVector<int> tEnvioImagenes;
};

#endif // CLIENT_H
