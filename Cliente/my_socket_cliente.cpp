#include "my_socket_cliente.h"
#include <QSslError>
#include <QDataStream>

My_Socket_Cliente::My_Socket_Cliente(QString dir_server, quint16 port_server, QString user_name, QString passwd , QString ruta_imagen, QObject *parent) :

    QObject(parent),
    username(user_name),
    password(passwd),
    ip_server(dir_server),
    server_port(port_server),
    my_ip("127.0.0.2"),
    my_port(3003),
    logeado(false)
{
    sslSocket = new QSslSocket();//creamos el socket que vamos a usar
    avatar = new QImage();
    avatar->load(ruta_imagen, "jpg");
    //sslSocket->ignoreSslErrors();
    sslSocket->setProtocol(QSsl::TlsV1_1);
    sslSocket->connectToHostEncrypted(ip_server, port_server);
    //qDebug() << sslSocket->socketDescriptor();
    //qDebug() << sslSocket->state();
    //connect(sslSocket, SIGNAL(connected()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(ready()));
    //connect(sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(error()));
    //sslSocket->bind(my_ip, my_port);//enlazamos socket a ip y puerto (propio)

}

void My_Socket_Cliente::ready()//solo para enviar mensajes al servidor(logearme, esperar mensajes,...)
{
    qDebug() << sslSocket->state();
    qDebug() << "First Connection";

    std::string mensaje;
    Message message;
    QByteArray pass = password.toUtf8();
    QByteArray ba;
    QBuffer buffer(&ba);
    //LISTA CON DIR DEL HOST
    //QList<QHostAddress> list = QNetworkInterface::allAddresses();

    //NOMBRE DEL USUARIO
    message.set_type(5);
    message.set_username(username.toStdString());
    //message.set_name_user(username.toStdString());

    QCryptographicHash *hash = new QCryptographicHash(QCryptographicHash::Md5);
    hash->addData(pass);

    QByteArray pass_encrypt = hash->result();


    //ENCRIPTACION DE LA CONTRASEÑA
    //pass = QCryptographicHash::hash(password.toLocal8Bit(), QCryptographicHash::Md5);

    //qDebug() << QString::fromStdString(pass.toStdString());
    //CONTRASEÑA DEL USUARIO
    message.set_ip(pass_encrypt.toHex().toStdString());
    //message.set_password(password.toStdString());

    //INFORMACION SOBRE EL HOST
    //message.set_ip_user(my_ip.toString().toStdString());//list.at(2) 2-->porque es la dir de la subred en la lista
    message.set_port(0);

    //IMAGEN
    avatar->save(&buffer, "jpg");
    message.set_avatar(ba.toBase64().data());

    //SERIALIZAMOS
    mensaje = message.SerializeAsString();

    QByteArray pkt(mensaje.c_str(), mensaje.size());
    //ENVIO del tamaño y paquete
    quint32 size_packet = pkt.size();
    qDebug() << size_packet;
    QByteArray envio;
    QDataStream env(&envio, QIODevice::WriteOnly);
    env.setVersion(7);
    env << (quint32)size_packet;
    qDebug() << envio.data();

    sslSocket->write(envio);
    sslSocket->write(pkt);




}

void My_Socket_Cliente::readyRead()//para cuando el servidor me reenvie los mensajes de otros user
{
    Message sms;
    QByteArray mensaje;
    qDebug() << "Llega mensaje";

    //TODO:leer mensaje que llega
    mensaje = sslSocket->readAll();

    //TODO2:deserializar
    sms.ParseFromString(mensaje.toStdString()); //guardamos en mensaje lo que deserializamos en sms.

    qDebug() << sms.type();

    if(sms.type() == 5){
        logeado = true;
    }else if(sms.type() == 2){

    }
}

void My_Socket_Cliente::error(){
    sslSocket->ignoreSslErrors();

    QString string=sslSocket->errorString();
    qDebug() << sslSocket->error();
    qDebug() << string;

}


