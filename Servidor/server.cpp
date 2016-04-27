#include "server.h"

Server::Server(QString dir, quint16 port, QObject *parent) :
    QObject(parent),
    servidor(nullptr),
    dir_(dir),
    port_(port)
{
    //db = new QSqlDatabase("./database.sqlite");
    *db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    db->setDatabaseName("./database.sqlite");
    servidor = new SslServer(dir_,port_,nullptr,nullptr,db,this);

    if(!db->open()){
        qDebug() << "No se pudo acceder a la base de datos\n";
        exit(1);
    }

}

void Server::run()
{
    servidor->listen(QHostAddress(dir_), port_);
}
