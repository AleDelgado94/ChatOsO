#include "client.h"

Client::Client(QTcpSocket *tcpSocket, QObject *parent) :
    QObject(parent),
    tcpSocket_(tcpSocket)
{
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Client::readyRead()
{
    QByteArray Buffer;
    Buffer = tcpSocket_->readAll();

    Message m;
    m.ParseFromString(Buffer.toStdString());


    switch(m.type()){
    case 0: //Crear Sala
    {
        QSqlDatabase *db = new QSqlDatabase("QSQLITE");
        db->setDatabaseName("database.sqlite");
        QSqlQuery query(*db);

        int port = m.port();
        std::string puerto;
        puerto = std::to_string(port);

        std::string creada = "SELECT * FROM " + m.salaname() + ";";
        query.exec(QString::fromStdString(creada));
        QString resultado = query.value(0).toString();

        if(resultado.isEmpty()){
            std::string crear_tabla = "CREATE TABLE " + m.salaname() + " (usuario VARCHAR(50) NOT NULL, puerto INT NOT NULL, direccion VARCHAR(50) PRIMARY KEY NOT NULL);";
            std::string crear_sala = "CREATE TABLE MESSAGE_" + m.salaname() + " (id INT PRIMARY KEY AUTOINCREMENT, usuario VARCHAR(50) NOT NULL, mensaje TEXT, foto TEXT);";
            std::string insertar_usuario = "INSERT INTO " + m.salaname() + " (usuario, puerto, direccion) VALUES (" + m.username() + ", " + puerto + ", " + m.ip() + ")";

            query.exec(QString::fromStdString(crear_tabla));
            query.exec(QString::fromStdString(crear_sala));
            query.exec(QString::fromStdString(insertar_usuario));
        }
        else{
            //EL USUARIO SE UNIRÁ A LA SALA
            std::string insertar_usuario = "INSERT INTO " + m.salaname() + " (usuario, puerto, direccion) VALUES (" + m.username() + ", " + puerto + ", " + m.ip() + ")";
            query.exec(QString::fromStdString(insertar_usuario));
            //TODO: Enviar X mensajes de la sala al usuario
        }

    }
        break;
    case 1:
    {
        //UNIRSE A UNA SALA YA CREADA
        QSqlDatabase *db = new QSqlDatabase("QSQLITE");
        db->setDatabaseName("database.sqlite");
        QSqlQuery query(*db);

        int port = m.port();
        std::string puerto;
        puerto = std::to_string(port);

        std::string creada = "SELECT * FROM " + m.salaname() + ";";
        query.exec(QString::fromStdString(creada));
        QString resultado = query.value(0).toString();

        //COMPROBAMOS SI NO ESTÁ CREADA
        if(!resultado.isEmpty()){
            //SI NO ESTÁ CREADA, LA CREAMOS
            std::string crear_tabla = "CREATE TABLE " + m.salaname() + " (usuario VARCHAR(50) NOT NULL, puerto INT NOT NULL, direccion VARCHAR(50) PRIMARY KEY NOT NULL);";
            std::string crear_sala = "CREATE TABLE MESSAGE_" + m.salaname() + " (id INT PRIMARY KEY AUTOINCREMENT, usuario VARCHAR(50) NOT NULL, mensaje TEXT, foto TEXT);";
            std::string insertar_usuario = "INSERT INTO " + m.salaname() + " (usuario, puerto, direccion) VALUES (" + m.username() + ", " + puerto + ", " + m.ip() + ");";

            query.exec(QString::fromStdString(crear_tabla));
            query.exec(QString::fromStdString(crear_sala));
            query.exec(QString::fromStdString(insertar_usuario));

        }
        else{
            //SIGNIFICA QUE ESTÁ CREADA
            std::string insertar_usuario = "INSERT INTO " + m.salaname() + " (usuario, puerto, direccion) VALUES (" + m.username() + ", " + puerto + ", " + m.ip() + ");";
            query.exec(QString::fromStdString(insertar_usuario));
            //TODO: Enviar X mensajes de la sala al usuario
        }
    }
        break;

    case 2:
    {
        //ENVIAR MENSAJE A UNA SALA
        //Reenviamos el mensaje a todos los usuarios de la sala
        //Metemos el mensaje en el historial
        QSqlDatabase *db = new QSqlDatabase("QSQLITE");
        db->setDatabaseName("database.sqlite");
        QSqlQuery query(*db);

        int port = m.port();
        std::string puerto;
        puerto = std::to_string(port);

        //ADD MESSAGE TO HISTORIAL
        std::string intoMESSAGEDB = "INSERT INTO MESSAGE_" + m.salaname() +" (usuario, mensaje, foto) VALUES ( " + m.username() + ", " + m.message() + ", );";
        query.exec(QString::fromStdString(intoMESSAGEDB));

        //REENVIO MENSAJE
        //COMPROBAMOS QUE USUARIOS (DIR + PUERTO) PERTENECEN A ESA SALA
        std::string usuarios_sala = "SELECT direccion, puerto FROM " + m.message() + ";";
        QString direccion;
        quint16 p;

        query.exec(QString::fromStdString(usuarios_sala));

        while(query.next()){
            //CONECTAMOS CON EL HOST Y LE ENVIAMOS LA INFORMACIÓN DEL MENSAJE QUE HEMOS RECIBIDO
            direccion = query.value("direccion").toString();
            p = query.value("puerto").toUInt();
            tcpSocket_->connectToHost(direccion, quint16(p));
            tcpSocket_->write(m.message().c_str(), strlen(m.message().c_str()));
        }


    }
        break;
    case 3:
        break;
    case 4:
    {
        QSqlDatabase *db = new QSqlDatabase("QSQLITE");
        db->setDatabaseName("database.sqlite");
        QSqlQuery query(*db);

        //DESCONEXIÓN. ELIMINACION DE LA SALA
        std::string eliminacion = "DELETE FROM " + m.salaname() + " WHERE usuario = " + m.username() + ";";
        query.exec(QString::fromStdString(eliminacion));

    }
        break;
    default: break;
    }

}
