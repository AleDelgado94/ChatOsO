#include "client.h"
#include <iostream>
#include <QDirIterator>
#include <QImageWriter>

Client::Client(QSslSocket *sslSocket, QSqlDatabase *db, QObject *parent) :
    QObject(parent),
    sslSocket_(sslSocket),
    db(db),
    tamPacket(0)
{
    qDebug() << "Creando Cliente";

    connect(sslSocket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(sslSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(sslSocket_, SIGNAL(sslErrors(QList<QSslError>)), sslSocket_, SLOT(ignoreSslErrors()));
    connect(sslSocket_, SIGNAL(disconnected()), sslSocket_, SLOT(deleteLater()));


    if(!db->open()){
        qDebug() << "Error al abrir la base de datos";
        exit(1);
    }

}

Message Client::deserializar()
{
    QByteArray buffer;
    Message paquete;

    while(sslSocket_->bytesAvailable() > 0){

             QDataStream in(sslSocket_);
             in.setVersion(QDataStream::Qt_4_0);
              if(sslSocket_->bytesAvailable() >= (int)(sizeof(qint32))&& (tamPacket==0))
              {

                  in >> tamPacket;
                  qDebug() << tamPacket;
              } if ((tamPacket !=0) && (sslSocket_->bytesAvailable() >=tamPacket )){
                 buffer=sslSocket_->read(tamPacket);
                 paquete.ParseFromString(buffer.toStdString());
                 tamPacket =0;
                 return paquete;


             }else{
                  qDebug() << "Menos bytes";
                  paquete.set_type(10);
                  return paquete;
             }
                //sslSocket_->readAll();
    }
    return paquete;
}

void Client::readyRead()
{

    while(sslSocket_->bytesAvailable()){
        Message m;
        m = deserializar();
        if(m.type() == 10)
            return;

        QSqlQuery query(*db);

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



                //qDebug() << "usuario: " << QString::fromStdString(m.username());
                //EL USUARIO SE UNIRÁ A LA SALA
                query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");

                query.exec("CREATE VIEW History AS SELECT usuario, mensaje, id FROM MESSAGE_" + QString::fromStdString(m.salaname()) + " ORDER BY id DESC LIMIT 10;");
                query.exec("SELECT usuario, mensaje FROM History ORDER BY id ASC;");

                while(query.next()){
                    Message reenvio;
                    reenvio.set_message(query.value("mensaje").toString().toStdString());
                    reenvio.set_type(2);
                    reenvio.set_username(query.value("usuario").toString().toStdString());
                    reenvio.set_port(0); //NO ME IMPORTA EL PUERTO
                    reenvio.set_ip(""); //NO ME IMPORTA LA DIRECCIÓN


                    //SERIALIZAMOS LA INFO
                    std::string mensaje_envio = reenvio.SerializeAsString();

                    QByteArray pkt(mensaje_envio.c_str(), mensaje_envio.size());
                    //ENVIO del tamaño y paquete
                    quint32 size_packet = pkt.size();
                    QByteArray envio;
                    QDataStream env(&envio, QIODevice::WriteOnly);
                    env.setVersion(7);
                    env << (quint32)size_packet;



                    //SERIALIZAMOS EL MENSAJE
                    //std::string message_reenvio = reenvio.SerializeAsString();
                    //qDebug() << QString::fromStdString(message_reenvio);

                    //ENVIAMOS LOS MENSAJES AL USUARIO
                    if(!mensaje_envio.empty()){
                        //sslSocket_->write(message_reenvio.c_str(), message_reenvio.length());
                        //ENVIO a los clientes
                        sslSocket_->write(envio);
                        sslSocket_->write(pkt);
                        //sslSocket_->waitForBytesWritten();
                    }
                }
                query.exec("DROP VIEW History");
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
                query.exec("CREATE TABLE "+ QString::fromStdString(m.salaname()) +" (usuario VARCHAR(50) PRIMARY KEY NOT NULL, puerto INT NOT NULL, direccion VARCHAR(50)  NOT NULL);");
                query.exec();
                query.executedQuery();


                QString message_sala = "MESSAGE_" + QString::fromStdString(m.salaname());

                //query.prepare("CREATE TABLE MESSAGE_:salaname (id INTEGER PRIMARY KEY AUTOINCREMENT, usuario VARCHAR(50) NOT NULL, mensaje TEXT, foto TEXT)");
                //query.bindValue(":salaname", QString::fromStdString(m.salaname()));
                query.exec("CREATE TABLE "+ message_sala + " (id INTEGER PRIMARY KEY AUTOINCREMENT, usuario VARCHAR(50) NOT NULL, mensaje TEXT, foto TEXT);");
                //qDebug() << query.exec();

                qDebug() << query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");
                //query.exec();
                qDebug() << query.executedQuery();
                qDebug() << "Error al insertar al usuario: " << query.lastError();


            }
            else{

                qDebug() << "USUARIO UNIENDOSE A LA SALA";

                qDebug() << "usuario: " << QString::fromStdString(m.username());
                //EL USUARIO SE UNIRÁ A LA SALA
                query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");
                //Enviar X mensajes de la sala al usuario

                query.exec("CREATE VIEW History AS SELECT usuario, mensaje, id FROM MESSAGE_" + QString::fromStdString(m.salaname()) + " ORDER BY id DESC LIMIT 10;");
                query.lastError();
                query.exec("SELECT usuario, mensaje FROM History ORDER BY id ASC;");


                while(query.next()){
                    Message reenvio;
                    reenvio.set_message(query.value("mensaje").toString().toStdString());
                    reenvio.set_type(2);
                    reenvio.set_username(query.value("usuario").toString().toStdString());
                    reenvio.set_port(0); //NO ME IMPORTA EL PUERTO
                    reenvio.set_ip(""); //NO ME IMPORTA LA DIRECCIÓN

                    //SERIALIZAMOS LA INFO
                    std::string mensaje_envio = reenvio.SerializeAsString();

                    QByteArray pkt(mensaje_envio.c_str(), mensaje_envio.size());
                    //ENVIO del tamaño y paquete
                    quint32 size_packet = pkt.size();
                    QByteArray envio;
                    QDataStream env(&envio, QIODevice::WriteOnly);
                    env.setVersion(7);
                    env << (quint32)size_packet;



                    //SERIALIZAMOS EL MENSAJE
                    //std::string message_reenvio = reenvio.SerializeAsString();
                    //qDebug() << QString::fromStdString(message_reenvio);

                    //ENVIAMOS LOS MENSAJES AL USUARIO
                    if(!mensaje_envio.empty()){
                        //sslSocket_->write(message_reenvio.c_str(), message_reenvio.length());
                        //ENVIO a los clientes
                        sslSocket_->write(envio);
                        sslSocket_->write(pkt);
                        //sslSocket_->waitForBytesWritten();

                    }
                }
                query.exec("DROP VIEW History");
            }

        }
            break;

        case 2:
        {
            //ENVIAR MENSAJE A UNA SALA
            //Reenviamos el mensaje a todos los usuarios de la sala
            //Metemos el mensaje en el historial


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
            qDebug() << "El mensaje a reenviar es: " << QString::fromStdString(mensaje);

            QByteArray pkt(mensaje.c_str(), mensaje.size());
            //ENVIO del tamaño y paquete
            quint32 size_packet = pkt.size();
            QByteArray envio;
            QDataStream env(&envio, QIODevice::WriteOnly);
            env.setVersion(7);
            env << (quint32)size_packet;



            while(query.next()){
                //CONECTAMOS CON EL HOST Y LE ENVIAMOS LA INFORMACIÓN DEL MENSAJE QUE HEMOS RECIBIDO
                usuario = query.value("usuario").toString();

                bool envia = false;
                if(list_clients.value(usuario))
                    envia = true;

                if(!mensaje.empty() && envia){
                    QSslSocket *socket = list_clients.value(usuario);

                    socket->write(envio);
                    socket->write(pkt);
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
            qDebug() << list_clients;
            list_clients.remove(QString::fromStdString(m.username()));
            qDebug() << list_clients;

        }
            break;
        case 5:
        {



            QSqlQuery query(*db);

            QString consulta =  "SELECT * FROM login WHERE usuario='" + QString::fromStdString(m.username()) +  "'  AND password='" + QString::fromStdString(m.ip()) + "';";


            query.exec(consulta);
            while(query.next()){
                if(!query.value("usuario").toString().isEmpty()){



                    qDebug() << "Recibiendo imagenes";
                    QBuffer* buffer = new QBuffer;
                    buffer->open(QIODevice::ReadWrite);
                    QByteArray b;
                    quint64 bytes = buffer->write(QByteArray::fromBase64(m.avatar().data()));

                    buffer->seek(buffer->pos() - bytes);
                    QImage image;
                    qDebug() << image.loadFromData(buffer->buffer(), "JPG");


                    QString ruta("/var/lib/ServidorChatOsO/Images/");
                    ruta +=  QString::fromStdString(m.username());
                    ruta += ".jpg";
                    QImageWriter img(ruta, "jpg");

                    img.write(image);

                    //MANDAR LA NUEVA IMAGEN A LOS USUARIOS CONECTADOS ANTERIORMENTE

                    QByteArray ba2;
                    QBuffer buffer2(&ba2);
                    image.save(&buffer2, "JPG");


                    Message confirmacion;
                    confirmacion.set_username(m.username());
                    confirmacion.set_ip("");
                    confirmacion.set_type(5);
                    confirmacion.set_port(0);
                    confirmacion.set_avatar(ba2.toBase64().data());


                    std::string message_confirm;
                    message_confirm = confirmacion.SerializeAsString();

                    QByteArray pkt(message_confirm.c_str(), message_confirm.size());
                    //ENVIO del tamaño y paquete
                    quint32 size_packet = pkt.size();
                    QByteArray envio;
                    QDataStream env(&envio, QIODevice::WriteOnly);
                    env.setVersion(7);
                    env << (quint32)size_packet;

                    QMap<QString, QSslSocket*>::iterator i;
                    for(i = list_clients.begin(); i != list_clients.end(); ++i){
                        qDebug() << "Recorriendo lista";

                        QSslSocket *socket = i.value();

                        socket->write(envio);
                        socket->write(pkt);

                    }

                    //**************************************************************

                    QDirIterator dirIt("/var/lib/ServidorChatOsO/Images/", QDirIterator::Subdirectories);
                    qDebug() << dirIt.path();
                    while(dirIt.hasNext()){
                        dirIt.next();
                        if(QFileInfo(dirIt.filePath()).isFile())

                            if(QFileInfo(dirIt.filePath()).suffix() == "jpg" || QFileInfo(dirIt.filePath()).suffix() == "jpeg" )
                            {
                                QString ruta = "/var/lib/ServidorChatOsO/Images/" + dirIt.fileName();
                                avatar = new QImage();
                                avatar->load(ruta, "JPG");
                                qDebug() << ruta;
                                QByteArray ba;
                                QBuffer buffer(&ba);
                                avatar->save(&buffer, "JPG");


                                Message confirmacion;
                                confirmacion.set_username(dirIt.fileName().toStdString());
                                confirmacion.set_ip("");
                                confirmacion.set_type(5);
                                confirmacion.set_port(0);
                                confirmacion.set_avatar(ba.toBase64().data());


                                std::string message_confirm;
                                message_confirm = confirmacion.SerializeAsString();

                                QByteArray pkt(message_confirm.c_str(), message_confirm.size());
                                //ENVIO del tamaño y paquete
                                quint32 size_packet = pkt.size();
                                QByteArray envio;
                                QDataStream env(&envio, QIODevice::WriteOnly);
                                env.setVersion(7);
                                env << (quint32)size_packet;

                                qDebug() << message_confirm.size();



                                if(!message_confirm.empty()){

                                    sslSocket_->write(envio);
                                    sslSocket_->write(pkt);
                                    list_clients.insert(QString::fromStdString(m.username()), sslSocket_);

                                }
                                qDebug() << list_clients;
                            }
                    }
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
