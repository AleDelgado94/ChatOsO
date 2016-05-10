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

    qDebug() << "Chivato" << sslSocket_->state();

    if(!db->open()){
        qDebug() << "Error al abrir la base de datos";
        exit(1);
    }

}

void Client::readyRead()
{

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

        QSqlQuery query(*db);

        int port = m.port();
        std::string puerto;
        puerto = std::to_string(port);

        std::string creada = "SELECT * FROM " + m.salaname() + ";";
        query.exec(QString::fromStdString(creada));
        QString resultado = query.value(0).toString();

        qDebug() << QString::fromStdString(m.salaname());
        qDebug() << QString::fromStdString(m.username());



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
            std::string insertar_usuario = "INSERT INTO " + m.salaname() + " (usuario, puerto, direccion) VALUES (" + m.username() + ", " + std::to_string(sslSocket_->socketDescriptor()) + ", " + m.ip() + ")";
            query.exec(QString::fromStdString(insertar_usuario));
            //TODO: Enviar X mensajes de la sala al usuario
        }

    }
        break;
    case 1:
    {

        //UNIRSE A UNA SALA YA CREADA

        //TODO: ENVIAR AL SERVIDOR MI AVATAR

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
            std::string insertar_usuario = "INSERT INTO " + m.salaname() + " (usuario, puerto, direccion) VALUES (" + m.username() + ", " + std::to_string(sslSocket_->socketDescriptor()) + ", " + m.ip() + ");";
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

        QSqlQuery query(*db);

        int port = m.port();
        std::string puerto;
        puerto = std::to_string(port);

        //ADD MESSAGE TO HISTORIAL
        std::string intoMESSAGEDB = "INSERT INTO MESSAGE_" + m.salaname() +" (usuario, mensaje, foto) VALUES ( " + m.username() + ", " + m.message() + ", );";
        query.exec(QString::fromStdString(intoMESSAGEDB));

        //REENVIO MENSAJE
        //COMPROBAMOS QUE USUARIOS (DIR + PUERTO) PERTENECEN A ESA SALA
        std::string usuarios_sala = "SELECT direccion, puerto FROM " + m.salaname() + " WHERE direccion != '" + m.ip() + "' AND puerto != " + puerto + ";";
        QString direccion;
        quint16 p;

        query.exec(QString::fromStdString(usuarios_sala));

        std::string mensaje;
        mensaje = m.SerializeAsString();

        while(query.next()){
            //CONECTAMOS CON EL HOST Y LE ENVIAMOS LA INFORMACIÓN DEL MENSAJE QUE HEMOS RECIBIDO
            direccion = query.value("direccion").toString();
            p = query.value("puerto").toUInt();

            if(!mensaje.empty()){

                //QSslSocket *socket = list_clients.key(p);
                //socket->write(mensaje.c_str(), qstrlen(mensaje.c_str()));

            }

        }


    }
        break;
    case 3:
        break;
    case 4:
    {

        QSqlQuery query(*db);

        //DESCONEXIÓN. ELIMINACION DE LA SALA
        std::string eliminacion = "DELETE FROM " + m.salaname() + " WHERE usuario = " + m.username() + ";";
        query.exec(QString::fromStdString(eliminacion));

    }
        break;
    case 5:
    {

        qDebug() << "Logeandose";
        qDebug() << Buffer;

        qDebug() << "Usuario: " << QString::fromStdString(m.username());
        qDebug() << "Pass: " << QString::fromStdString(m.ip());

        QSqlQuery query(*db);

        //query.prepare("SELECT * FROM login WHERE usuario=':user'  AND password=':pass';");
        //query.bindValue(":user", QString::fromStdString(m.username()));
        //query.bindValue(":pass", QString::fromStdString(m.ip()));

        QString consulta =  "SELECT * FROM login WHERE usuario='" + QString::fromStdString(m.username()) +  "'  AND password='" + QString::fromStdString(m.ip()) + "';";


        query.exec(consulta);
        while(query.next()){
            if(!query.value("usuario").toString().isEmpty()){
                //SIGNIFICA QUE EL USUARIO ESTÁ EN LA BASE DE DATOS
                //TODO: Enviar todas las fotos de los usuarios, mensaje OK
                /*
                QHostAddress ip(QString::fromStdString(message.ip_user()));

                //QHostAddress ip = sslSocket_->peerAddress();
                //quint16 port = sslSocket_->peerPort();
                quint16 port = message.port_user();

                //METO EN LA BASE DE DATOS EL PUERTO Y LA DIRECCION
                query.prepare("UPDATE login SET ip = ':dir_ip' WHERE usuario=':user'");
                query.bindValue(":user", QString::fromStdString(m.username()));
                query.bindValue(":dir_ip",ip.toString());
                query.exec();

                query.prepare("UPDATE login SET port=:puerto WHERE usuario=':user'");
                query.bindValue(":user", QString::fromStdString(message.name_user()));
                query.bindValue(":puerto",port);
                query.exec();
                */

                qDebug() << "Entra consulta";

                Message confirmacion;
                confirmacion.set_username("");
                confirmacion.set_ip("");
                confirmacion.set_type(5);
                confirmacion.set_port(0);

                std::string message_confirm;
                message_confirm = confirmacion.SerializeAsString();

                if(!message_confirm.empty())
                    sslSocket_->write(message_confirm.c_str(), qstrlen(message_confirm.c_str()));

                list_clients.insert(sslSocket_, QString::fromStdString(m.username()));
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
