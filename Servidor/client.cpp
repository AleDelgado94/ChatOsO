#include "client.h"

Client::Client(QSslSocket *sslSocket, QSqlDatabase *db, QObject *parent) :
    QObject(parent),
    sslSocket_(sslSocket),
    db(db)
{

    qDebug() << "Creando Cliente";

    connect(sslSocket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //connect(sslSocket_, SIGNAL(connected()), this, SLOT(firstConnection()));
    //connect(sslSocket_, SIGNAL(encrypted()), this, SLOT(firstConnection()));
    connect(sslSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(sslSocket_, SIGNAL(sslErrors(QList<QSslError>)), sslSocket_, SLOT(ignoreSslErrors()));
    connect(sslSocket_, SIGNAL(disconnected()), sslSocket_, SLOT(deleteLater()));


    if(!db->open()){
        qDebug() << "Error al abrir la base de datos";
        exit(1);
    }

}

void Client::readyRead()
{
    QSqlQuery query(*db);


    qDebug() << "Ready read()";

    QByteArray Buffer;


    Buffer = sslSocket_->readAll();
    qDebug() << Buffer;


    //TODO: IMPLEMENTAR EL DESERIALIZADO CON EL TAMAÑO DEL PAQUETE



    Message m;
    m.ParseFromString(Buffer.toStdString());
    qDebug() << QString::fromStdString(m.username());


    switch(m.type()){
    case 0: //Crear Sala
    {


        bool exist = false;
        QStringList a = db->tables(QSql::Tables);

        if(a.contains(QString::fromStdString(m.salaname()))){
            exist = true;
        }


        qDebug() << "case 0";


        if(!exist){


            qDebug() << query.exec("CREATE TABLE "+ QString::fromStdString(m.salaname()) +" (usuario VARCHAR(50) PRIMARY KEY NOT NULL, puerto INT NOT NULL, direccion VARCHAR(50)  NOT NULL);");
            qDebug() << query.exec();
            qDebug() << query.executedQuery();


            QString message_sala = "MESSAGE_" + QString::fromStdString(m.salaname());
            qDebug() << message_sala;

            qDebug() << query.exec("CREATE TABLE "+ message_sala + " (id INTEGER PRIMARY KEY AUTOINCREMENT, usuario VARCHAR(50) NOT NULL, mensaje TEXT, foto TEXT);");
            //qDebug() << query.exec();
            qDebug() << query.executedQuery();
            qDebug() << "Error al crear la tabla mensajes: " << query.lastError();



            qDebug() << query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");
            qDebug() << query.executedQuery();
            qDebug() << "Error al insertar al usuario: " << query.lastError();


        }
        else{

            qDebug() << "USUARIO UNIENDOSE A LA SALA";



            qDebug() << "usuario: " << QString::fromStdString(m.username());
            //EL USUARIO SE UNIRÁ A LA SALA
            qDebug() << query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");
            qDebug() << query.lastError();
            //TODO: Enviar X mensajes de la sala al usuario
        }

    }
        break;
    case 1:
    {

        //UNIRSE A UNA SALA YA CREADA

        //TODO: ENVIAR AL SERVIDOR MI AVATAR

        bool exist = false;
        QStringList a = db->tables(QSql::Tables);

        if(a.contains(QString::fromStdString(m.salaname()))){
            exist = true;
        }


        qDebug() << "case 0";


        if(!exist){


            qDebug() << "Creando tablas e insertando";
            //query.prepare("CREATE TABLE :salaname (usuario VARCHAR(50) PRIMARY KEY NOT NULL, puerto INT NOT NULL, direccion VARCHAR(50)  NOT NULL)");
            //query.bindValue(":salaname", QString::fromStdString(m.salaname()));
            qDebug() << query.exec("CREATE TABLE "+ QString::fromStdString(m.salaname()) +" (usuario VARCHAR(50) PRIMARY KEY NOT NULL, puerto INT NOT NULL, direccion VARCHAR(50)  NOT NULL);");
            qDebug() << query.exec();
            qDebug() << query.executedQuery();


            QString message_sala = "MESSAGE_" + QString::fromStdString(m.salaname());
            qDebug() << message_sala;

            //query.prepare("CREATE TABLE MESSAGE_:salaname (id INTEGER PRIMARY KEY AUTOINCREMENT, usuario VARCHAR(50) NOT NULL, mensaje TEXT, foto TEXT)");
            //query.bindValue(":salaname", QString::fromStdString(m.salaname()));
            qDebug() << query.exec("CREATE TABLE "+ message_sala + " (id INTEGER PRIMARY KEY AUTOINCREMENT, usuario VARCHAR(50) NOT NULL, mensaje TEXT, foto TEXT);");
            //qDebug() << query.exec();
            qDebug() << query.executedQuery();
            qDebug() << "Error al crear la tabla mensajes: " << query.lastError();


            //query.prepare("INSERT INTO :salaname (usuario, puerto, direccion) VALUES (:username, :puerto, :ip)");
            //query.bindValue(":salaname", QString::fromStdString(m.salaname()));
            //query.bindValue(":username", QString::fromStdString(m.username()));
            //query.bindValue(":puerto", m.port());
            //query.bindValue(":ip", QString::fromStdString(m.ip()));
            qDebug() << query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");
            //query.exec();
            qDebug() << query.executedQuery();
            qDebug() << "Error al insertar al usuario: " << query.lastError();


        }
        else{

            qDebug() << "USUARIO UNIENDOSE A LA SALA";

            qDebug() << "usuario: " << QString::fromStdString(m.username());
            //EL USUARIO SE UNIRÁ A LA SALA
            qDebug() << query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");
            qDebug() << query.lastError();
            //TODO: Enviar X mensajes de la sala al usuario
        }

    }
        break;

    case 2:
    {
        //ENVIAR MENSAJE A UNA SALA
        //Reenviamos el mensaje a todos los usuarios de la sala
        //Metemos el mensaje en el historial

        QSqlQuery query(*db);

        int port = m.port();
        std::string puerto;
        puerto = std::to_string(port);

        //ADD MESSAGE TO HISTORIAL
        std::string intoMESSAGEDB = "INSERT INTO MESSAGE_" + m.salaname() +" (usuario, mensaje, foto) VALUES ( '" + m.username() + "', '" + m.message() + "', '');";
        query.exec(QString::fromStdString(intoMESSAGEDB));

        //REENVIO MENSAJE
        //COMPROBAMOS QUE USUARIOS (DIR + PUERTO) PERTENECEN A ESA SALA
        std::string usuarios_sala = "SELECT usuario FROM " + m.salaname() + " WHERE usuario != '" + m.username() + "';";
        QString usuario;
        quint16 p;

        query.exec(QString::fromStdString(usuarios_sala));

        std::string mensaje;
        mensaje = m.SerializeAsString();

        while(query.next()){
            //CONECTAMOS CON EL HOST Y LE ENVIAMOS LA INFORMACIÓN DEL MENSAJE QUE HEMOS RECIBIDO
            usuario = query.value("usuario").toString();


            if(!mensaje.empty()){

                QSslSocket *socket = list_clients.value(usuario);
                socket->write(mensaje.c_str(), mensaje.length());

            }

        }


    }
        break;
    case 3:
        break;
    case 4:
    {


        //DESCONEXIÓN. ELIMINACION DE LA SALA


        qDebug() << query.exec("DELETE FROM " + QString::fromStdString(m.salaname()) + " where usuario='" + QString::fromStdString(m.username()) + "';");
        sslSocket_->disconnect();
        list_clients.remove(QString::fromStdString(m.username()));
        qDebug() << list_clients;

    }
        break;
    case 5:
    {

        qDebug() << "Logeandose";
        qDebug() << Buffer;

        qDebug() << "Usuario: " << QString::fromStdString(m.username());
        qDebug() << "Pass: " << QString::fromStdString(m.ip());

        QSqlQuery query(*db);

        QString consulta =  "SELECT * FROM login WHERE usuario='" + QString::fromStdString(m.username()) +  "'  AND password='" + QString::fromStdString(m.ip()) + "';";


        query.exec(consulta);
        while(query.next()){
            if(!query.value("usuario").toString().isEmpty()){

                Message confirmacion;
                confirmacion.set_username("");
                confirmacion.set_ip("");
                confirmacion.set_type(5);
                confirmacion.set_port(0);

                std::string message_confirm;
                message_confirm = confirmacion.SerializeAsString();

                if(!message_confirm.empty())
                    sslSocket_->write(message_confirm.c_str(), qstrlen(message_confirm.c_str()));

                list_clients.insert(QString::fromStdString(m.username()), sslSocket_);
                qDebug() << list_clients;
            }else{
                sslSocket_->disconnectFromHost();
                sslSocket_->close();
            }
        }


    }
        break;
    default: break;
    }

}

void Client::error(){
    qDebug() << "Error\n";
}
/*
void Client::firstConnection()
{

    qDebug() << "Conexión entrante";
    //TODO: Deserializar con el tamaño del paquete al inicio

    QByteArray buffer;
    buffer = sslSocket_->readAll();
    qDebug() << buffer;

    Message_Log message;
    message.ParseFromString(buffer.toStdString());


    QSqlQuery query(*db);

    query.prepare("SELECT * FROM login WHERE usuario=':user' "
                  "AND password=':pass'");
    query.bindValue(":user", QString::fromStdString(message.name_user()));
    query.bindValue(":pass", QString::fromStdString(message.password()));

    query.exec();

    if(!query.value("usuario").toString().isEmpty()){
        //SIGNIFICA QUE EL USUARIO ESTÁ EN LA BASE DE DATOS
        //TODO: Enviar todas las fotos de los usuarios, mensaje OK

        QHostAddress ip(QString::fromStdString(message.ip_user()));

        //QHostAddress ip = sslSocket_->peerAddress();
        //quint16 port = sslSocket_->peerPort();
        quint16 port = message.port_user();

        //METO EN LA BASE DE DATOS EL PUERTO Y LA DIRECCION
        query.prepare("UPDATE login SET ip = ':dir_ip' WHERE usuario=':user'");
        query.bindValue(":user", QString::fromStdString(message.name_user()));
        query.bindValue(":dir_ip",ip.toString());
        query.exec();

        query.prepare("UPDATE login SET port=:puerto WHERE usuario=':user'");
        query.bindValue(":user", QString::fromStdString(message.name_user()));
        query.bindValue(":puerto",port);
        query.exec();



        sslSocket_->write("OK");
        list_clients.insert(sslSocket_, QString::fromStdString(message.name_user()));
    }else{
        sslSocket_->disconnectFromHost();
        sslSocket_->close();
    }

}
*/
