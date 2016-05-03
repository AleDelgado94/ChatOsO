#include "my_socket_cliente.h"


My_Socket_Cliente::My_Socket_Cliente(QString dir_server, quint16 port_server, QString user_name, QString passwd ,QObject *parent) :

    QObject(parent),
    username(user_name),
    password(passwd),
    ip_server(dir_server),
    server_port(port_server),
    my_ip("127.0.0.1"),
    my_port(3003),
    logeado(false)
{
    sslSocket = new QSslSocket(this);//creamos el socket que vamos a usar
    sslSocket->bind(my_ip, my_port);//enlazamos socket a ip y puerto (propio)
    sslSocket->connectToHostEncrypted(dir_server, port_server);
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void My_Socket_Cliente::ready()//solo para enviar mensajes al servidor(logearme, esperar mensajes,...)
{

    std::string mensaje;
    Message_Log message;
    QByteArray pass = password.toUtf8();
    QByteArray buffer;

    //LISTA CON DIR DEL HOST
    //QList<QHostAddress> list = QNetworkInterface::allAddresses();

    //NOMBRE DEL USUARIO
    message.set_name_user(username.toStdString());
    //CONTRASEÑA DEL USUARIO
    message.set_password(password.toStdString());

    //ENCRIPTACION DE LA CONTRASEÑA
    pass = QCryptographicHash::hash(password.toLocal8Bit(), QCryptographicHash::Md5);

    //INFORMACION SOBRE EL HOST
    message.set_ip_user(my_ip.toString().toStdString());//list.at(2) 2-->porque es la dir de la subred en la lista
    message.set_port_user(my_port);

    //SERIALIZAMOS
    mensaje = message.SerializeAsString();

    //ENVIO
    sslSocket->write(mensaje.c_str(), qstrlen(mensaje.c_str()));
    sslSocket->waitForReadyRead(300000); //ESPERAMOS RESPUESTA DEL SERVIDOR(30sec)

    //LEEMOS MENSAJE DEL SERVIDOR Y VEMOS SI PODEMOS ENTRAR O NO
    buffer = sslSocket->readAll();
     if(buffer == "OK")
         logeado = true;


}

void My_Socket_Cliente::readyRead()//para cunado el servidor me reenvie los mensajes de otros user
{
    Message sms;
    std::string mensaje;

    //TODO:leer mensaje que llega
    sslSocket->readAll();

    //TODO2:deserializar
    sms.ParseFromString(mensaje); //guardamos en mensaje lo que deserializamos en sms.

    //TODO3:mostrar mensaje por pantalla


    //TODO4:Ir a la carpeta con avatares y mostrarlo
    //TODO5:envio de imagen Qbuffer
}


