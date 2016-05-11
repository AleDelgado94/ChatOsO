#include "my_socket_cliente.h"
#include <QSslError>


My_Socket_Cliente::My_Socket_Cliente(QString dir_server, quint16 port_server, QString user_name, QString passwd ,QObject *parent) :

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
    //sslSocket->ignoreSslErrors();
    sslSocket->setProtocol(QSsl::TlsV1_1);
    sslSocket->connectToHostEncrypted(ip_server, port_server);
    qDebug() << sslSocket->socketDescriptor();
    qDebug() << sslSocket->state();

    //connect(sslSocket, SIGNAL(connected()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
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
    QByteArray buffer;

    //LISTA CON DIR DEL HOST
    //QList<QHostAddress> list = QNetworkInterface::allAddresses();

    //NOMBRE DEL USUARIO
    message.set_type(5);
    message.set_username(username.toStdString());
    //message.set_name_user(username.toStdString());

    //ENCRIPTACION DE LA CONTRASEÑA
    pass = QCryptographicHash::hash(password.toLocal8Bit(), QCryptographicHash::Md5);
    //CONTRASEÑA DEL USUARIO
    message.set_ip(password.toStdString());
    //message.set_password(password.toStdString());



    //INFORMACION SOBRE EL HOST
    //message.set_ip_user(my_ip.toString().toStdString());//list.at(2) 2-->porque es la dir de la subred en la lista
    message.set_port(0);

    //SERIALIZAMOS
    mensaje = message.SerializeAsString();

    qDebug() << QString::fromStdString(mensaje);

    //ENVIO

    sslSocket->write(mensaje.c_str(), qstrlen(mensaje.c_str()));
    //sslSocket->waitForReadyRead(300000); //ESPERAMOS RESPUESTA DEL SERVIDOR(30sec)

    //LEEMOS MENSAJE DEL SERVIDOR Y VEMOS SI PODEMOS ENTRAR O NO
    //buffer = sslSocket->readAll();
     //if(buffer == "OK")
       //  logeado = true;


}

void My_Socket_Cliente::readyRead()//para cunado el servidor me reenvie los mensajes de otros user
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
    }else{

    }

    //TODO3:mostrar mensaje por pantalla


    //TODO4:Ir a la carpeta con avatares y mostrarlo
    //TODO5:envio de imagen Qbuffer
}

void My_Socket_Cliente::error(){
    sslSocket->ignoreSslErrors();

    QString string=sslSocket->errorString();
    qDebug() << sslSocket->error();
    qDebug() << string;

}

