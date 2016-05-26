#include "client.h"
#include <iostream>
#include <QDirIterator>
#include <QImageWriter>
#include <syslog.h>
#include <unistd.h>
#include <QDateTime>




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
    connect(sslSocket_, SIGNAL(disconnected()), this, SLOT(statistics()));


    if(!db->open()){
        qDebug() << "Error al abrir la base de datos";
        exit(1);
    }

}


Message Client::deserializar()
{
    QByteArray buffer;
    Message paquete;

    qint64 clock_receipt1 = QDateTime::currentMSecsSinceEpoch();
    while(sslSocket_->bytesAvailable() > 0){


             QDataStream in(sslSocket_);
             in.setVersion(QDataStream::Qt_4_0);
              if(sslSocket_->bytesAvailable() >= (int)(sizeof(qint32))&& (tamPacket==0))
              {

                  in >> tamPacket;
                  qDebug() << "El tamaño del paquete es: " << tamPacket;
              } if ((tamPacket !=0) && (sslSocket_->bytesAvailable() >=tamPacket )){
                 buffer=sslSocket_->read(tamPacket);
                 paquete.ParseFromString(buffer.toStdString());
                 tamPacket =0;
                 quint64 clock_receipt2 = QDateTime::currentMSecsSinceEpoch() - clock_receipt1;
                 tRecepcionPaquetes.append(clock_receipt2);

                 qDebug() << "bytes leidos completo";
                 return paquete;


             }else{
                  qDebug() << "Menos bytes";
                  paquete.set_type(10);
                  return paquete;
             }
                //sslSocket_->readAll();
    }
    paquete.set_type(10);
    return paquete;
}

void Client::readyRead()
{


    while(sslSocket_->bytesAvailable()){

        QTime receiptPaquete;
        receiptPaquete.start();

        //qint64 clock_receipt = QDateTime::currentMSecsSinceEpoch();

        Message m;
        m = deserializar();

        qDebug() << m.type();

        //clock_receipt = QDateTime::currentMSecsSinceEpoch() - clock_receipt;
        //tRecepcionPaquetes.append(clock_receipt);

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


            if(!exist){



                QString log = "Creando la sala " + QString::fromStdString(m.salaname()) + "\n";
                syslog(LOG_NOTICE, log.toStdString().c_str());

                QTime tDB;
                tDB.start();

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

                if(tDB.elapsed() > 1)
                    tConsultasDB.append(tDB.elapsed());

            }
            else{

                QString log = "Usuario " + QString::fromStdString(m.username())+ " uniendose a la sala " + QString::fromStdString(m.salaname()) + "\n";
                syslog(LOG_NOTICE, log.toStdString().c_str());

                QTime tDB;
                tDB.start();

                //EL USUARIO SE UNIRÁ A LA SALA
                query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");

                query.exec("CREATE VIEW History AS SELECT usuario, mensaje, id FROM MESSAGE_" + QString::fromStdString(m.salaname()) + " ORDER BY id DESC LIMIT 10;");
                query.exec("SELECT usuario, mensaje FROM History ORDER BY id ASC;");

                tConsultasDB.append(tDB.elapsed());

                QTime tEnvioMensajes;
                tEnvioMensajes.start();

                while(query.next()){
                    qDebug() << "&================";
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

                    //ENVIAMOS LOS MENSAJES AL USUARIO
                    if(!mensaje_envio.empty()){
                        //ENVIO a los clientes
                        sslSocket_->write(envio);
                        sslSocket_->write(pkt);
                    }



                }

                if(tEnvioMensajes.elapsed() > 1){
                    qDebug() << "Envio de mensajes time: " << tEnvioMensajes.elapsed();
                    tEnvioPaquetes.append(tEnvioMensajes.elapsed());
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


            if(!exist){

                QString log = "Creando la sala " + QString::fromStdString(m.salaname()) + "\n";
                syslog(LOG_NOTICE, log.toStdString().c_str());

                QTime tDB;
                tDB.start();

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

                if(tDB.elapsed() > 1)
                    tConsultasDB.append(tDB.elapsed());

            }
            else{

                QString log = "Usuario " + QString::fromStdString(m.username())+ " uniendose a la sala " + QString::fromStdString(m.salaname()) + "\n";
                syslog(LOG_NOTICE, log.toStdString().c_str());

                QTime tDB;
                tDB.start();

                //EL USUARIO SE UNIRÁ A LA SALA
                query.exec("INSERT INTO " + QString::fromStdString(m.salaname()) + " (usuario, puerto, direccion) VALUES ('" + QString::fromStdString(m.username()) +"', '" + m.port() + "', '" + QString::fromStdString(m.ip()) + "');");
                //Enviar X mensajes de la sala al usuario

                query.exec("CREATE VIEW History AS SELECT usuario, mensaje, id FROM MESSAGE_" + QString::fromStdString(m.salaname()) + " ORDER BY id DESC LIMIT 10;");
                query.exec("SELECT usuario, mensaje FROM History ORDER BY id ASC;");

                tConsultasDB.append(tDB.elapsed());

                QTime tEnvioMensajes;
                tEnvioMensajes.start();

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

                    qDebug() << reenvio.type();
                    qDebug() << QString::fromStdString(mensaje_envio);

                    //ENVIAMOS LOS MENSAJES AL USUARIO
                    if(!mensaje_envio.empty()){
                        //ENVIO a los clientes
                        sslSocket_->write(envio);
                        sslSocket_->write(pkt);

                    }
                }
                qDebug() << "Tiempo de los mensajes: " << tEnvioMensajes.elapsed();
                if(tEnvioMensajes.elapsed() >= 0){
                    qDebug() << "Tiempo al reenviar los mensajes" << tEnvioMensajes.elapsed();
                    tEnvioPaquetes.append(tEnvioMensajes.elapsed());
                }
                query.exec("DROP VIEW History");
            }

        }
            break;

        case 2:
        {
            //ENVIAR MENSAJE A UNA SALA

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

            qint64 clock = QDateTime::currentMSecsSinceEpoch();
            QTime tEnvioMensajes;
            tEnvioMensajes.start();

            query.exec(QString::fromStdString(usuarios_sala));

            std::string mensaje;
            mensaje = m.SerializeAsString();
            qDebug() << "TIPO DEL MEN: " << m.type();
            qDebug() << "El mensaje a reenviar es: " << QString::fromStdString(mensaje);
            qDebug() << list_clients;


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
            clock = QDateTime::currentMSecsSinceEpoch()-clock;
                tEnvioPaquetes.append(clock);

            qDebug() << "Tiempo reenvio de mensajes entre usuarios: " << clock;


        }
            break;
        case 3:
            break;
        case 4:
        {

            QString log = "Usuario " + QString::fromStdString(m.username())+ " saliendo de la sala " + QString::fromStdString(m.salaname()) + "\n";
            syslog(LOG_NOTICE, log.toStdString().c_str());
            //DESCONEXIÓN. ELIMINACION DE LA SALA

            qDebug() << query.exec("DELETE FROM " + QString::fromStdString(m.salaname()) + " where usuario='" + QString::fromStdString(m.username()) + "';");
            //sslSocket_->disconnect();
            sslSocket_->disconnectFromHost();
            sslSocket_->close();
            //sslSocket_->disconnectFromHost();
            qDebug() << list_clients;
            list_clients.remove(QString::fromStdString(m.username()));
            qDebug() << list_clients;

        }
            break;
        case 5:
        {



            QSqlQuery query(*db);

            QString consulta =  "SELECT usuario FROM login WHERE usuario='" + QString::fromStdString(m.username()) +  "'  AND password='" + QString::fromStdString(m.ip()) + "' ORDER BY usuario LIMIT 1;";


            qDebug() << query.exec(consulta);


            //qDebug() << query.value(0).toString();
            query.seek(0);

            if(query.value(0).toString().isEmpty()){
                Message confirmacion;
                confirmacion.set_username(m.username());
                confirmacion.set_ip("");
                confirmacion.set_type(12);
                confirmacion.set_port(0);


                std::string message_confirm;
                message_confirm = confirmacion.SerializeAsString();

                QByteArray pkt(message_confirm.c_str(), message_confirm.size());
                //ENVIO del tamaño y paquete
                quint32 size_packet = pkt.size();
                QByteArray envio;
                QDataStream env(&envio, QIODevice::WriteOnly);
                env.setVersion(7);
                env << (quint32)size_packet;

                if(!message_confirm.empty()){
                    sslSocket_->write(envio);
                    sslSocket_->write(pkt);
                }

                sslSocket_->disconnectFromHost();
                sslSocket_->close();
            }
            else{
            //if(query.first()){


            if(!query.value("usuario").toString().isEmpty()){

                //MANDAMOS EL MENSAJE DE CONFIRMACIÓN PARA QUE EL USUARIO PUEDA ACCEDER A LA VENTANA DEL CHAT

                Message confirmacion1;
                confirmacion1.set_username(m.username());
                confirmacion1.set_ip("");
                confirmacion1.set_type(11);
                confirmacion1.set_port(0);


                std::string message_confirm1;
                message_confirm1 = confirmacion1.SerializeAsString();

                QByteArray pkt1(message_confirm1.c_str(), message_confirm1.size());
                //ENVIO del tamaño y paquete
                quint32 size_packet1 = pkt1.size();
                QByteArray envio1;
                QDataStream env1(&envio1, QIODevice::WriteOnly);
                env1.setVersion(7);
                env1 << (quint32)size_packet1;

                if(!message_confirm1.empty()){
                    sslSocket_->write(envio1);
                    sslSocket_->write(pkt1);
                    list_clients.insert(QString::fromStdString(m.username()), sslSocket_);
                }
                qDebug() << list_clients;

                //RECIBIMOS IMAGENES


                    qDebug() << "Recibiendo imagenes";
                    QBuffer* buffer = new QBuffer;
                    buffer->open(QIODevice::ReadWrite);
                    QByteArray b;
                    quint64 bytes = buffer->write(QByteArray::fromBase64(m.avatar().data()));

                    buffer->seek(buffer->pos() - bytes);
                    QImage image;
                    image.loadFromData(buffer->buffer(), "JPG");


                    QString ruta("/var/lib/ServidorChatOsO/Images/");
                    ruta +=  QString::fromStdString(m.username());
                    ruta += ".jpg";
                    qDebug() << "La ruta de la imagen es: " << ruta;
                    QImageWriter img(ruta, "jpg");

                    img.write(image);

                    //MANDAR LA NUEVA IMAGEN A LOS USUARIOS CONECTADOS ANTERIORMENTE

                    QByteArray ba2;
                    QBuffer buffer2(&ba2);
                    image.save(&buffer2, "JPG");


                    Message confirmacion;
                    confirmacion.set_username(m.username()+".jpg");
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
                        QSslSocket *socket = i.value();

                        socket->write(envio);
                        socket->write(pkt);

                    }

                    //**************************************************************

                    QDirIterator dirIt("/var/lib/ServidorChatOsO/Images/", QDirIterator::Subdirectories);
                    QTime tEnvImages;
                    tEnvImages.start();
                    while(dirIt.hasNext()){
                        dirIt.next();
                        if(QFileInfo(dirIt.filePath()).isFile())
                            if(QFileInfo(dirIt.filePath()).suffix() == "jpg" || QFileInfo(dirIt.filePath()).suffix() == "jpeg" )
                            {
                                QString ruta = "/var/lib/ServidorChatOsO/Images/" + dirIt.fileName();
                                avatar = new QImage();
                                avatar->load(ruta, "JPG");
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
                                }
                            }
                    }
                    if(tEnvImages.elapsed() > 1)
                        tEnvioImagenes.append(tEnvImages.elapsed());
                    qDebug() << tEnvImages.elapsed();




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

void Client::statistics()
{
    QString ruta_estadistica("/var/lib/ServidorChatOsO/estadisticas/");
    ruta_estadistica += QString::number(sslSocket_->socketDescriptor());
    ruta_estadistica += ".txt";
    QFile estadisticas(ruta_estadistica);

    qDebug() << tRecepcionPaquetes;
    qDebug() << tConsultasDB;
    qDebug() << tEnvioImagenes;
    qDebug() << tEnvioPaquetes;

    if(estadisticas.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream stat(&estadisticas);

        stat << "TIEMPOS ms\n";
        stat << "PRUEBAS: " << NumPruebas << "\n";

        int tRecibido = 0;
        for(int i=0; i<tRecepcionPaquetes.size() && i<NumPruebas; i++){
            tRecibido += tRecepcionPaquetes[i];
        }

        stat << "Tiempo medio Recepción de mensaje: " << tRecibido << "\n";

        int tConsultas = 0;
        for(int i=0; i<tConsultasDB.size() && i<NumPruebas; i++){
            tConsultas += tConsultasDB[i];
        }

        stat << "Tiempo medio Consultas en la Base de Datos: " << tConsultas/NumPruebas << "\n";

        int tImagenes = 0;
        for(int i=0; i<tEnvioImagenes.size() && i<NumPruebas; i++){
            tImagenes += tEnvioImagenes[i];
        }

        stat << "Tiempo Envio de Imagenes: " << tImagenes << "\n";

        int tEnvPaquetes = 0;
        for(int i=0; i<tEnvioPaquetes.size() && i<NumPruebas; i++){
            tEnvPaquetes += tEnvioPaquetes[i];
        }

        stat << "Tiempo Envio de Mensajes Historial : " << tEnvPaquetes << "\n";
    }
    estadisticas.close();

}
