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
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(error()));

}

void My_Socket_Cliente::ready()//solo para enviar mensajes al servidor(logearme, esperar mensajes,...)
{

    std::string mensaje;
    Message message;
    QByteArray pass = password.toUtf8();
    QByteArray ba;
    QBuffer buffer(&ba);

    //NOMBRE DEL USUARIO
    message.set_type(5);
    message.set_username(username.toStdString());

    QCryptographicHash *hash = new QCryptographicHash(QCryptographicHash::Md5);
    hash->addData(pass);

    QByteArray pass_encrypt = hash->result();


    //ENCRIPTACION DE LA CONTRASEÑA
    //CONTRASEÑA DEL USUARIO
    message.set_ip(pass_encrypt.toHex().toStdString());
    message.set_port(0);

    //IMAGEN
    avatar->save(&buffer, "jpg");
    message.set_avatar(ba.toBase64().data());

    //SERIALIZAMOS
    mensaje = message.SerializeAsString();

    QByteArray pkt(mensaje.c_str(), mensaje.size());
    //ENVIO del tamaño y paquete
    quint32 size_packet = pkt.size();
    QByteArray envio;
    QDataStream env(&envio, QIODevice::WriteOnly);
    env.setVersion(7);
    env << (quint32)size_packet;

    sslSocket->write(envio);
    sslSocket->write(pkt);




}


void My_Socket_Cliente::error(){
    sslSocket->ignoreSslErrors();
}


