#include "my_socket_cliente.h"


My_Socket_Cliente::My_Socket_Cliente(QString dir_server, quint16 port_server, QString user_name, QString passwd ,QObject *parent) :

    QObject(parent),
    username(user_name),
    password(passwd)
{
    sslSocket = new QSslSocket(this);//creamos el socket que vamos a usar
    sslSocket->connectToHostEncrypted(dir_server, port_server);
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(ready()));
    connect(sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void My_Socket_Cliente::ready()//solo para enviar mensajes al servidor(logearme, esperar mensajes,...)
{

    std::string mensaje;
    Log login_user;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    //NOMBRE Y CONTRASEÑA DEL USUARIO
    login_user.set_name_user(username.toStdString());
    login_user.set_password(password.toStdString());

    //INFORMACION SOBRE EL HOST
    login_user.set_ip_user(list.at(2).toString().toStdString());//2: ip de la red en la que estamos incluida en nuestra lista list.
    login_user.set_port_user(3000);

    //llamada a la funcion de encriptar contraseña(falta)

    //SERIALIZAMOS
    mensaje = login_user.SerializeAsString();

    //ENVIO
    sslSocket->write(mensaje.c_str(), qstrlen(mensaje.c_str()));

}

void My_Socket_Cliente::readyRead()//para cunado el servidor me reenvie los mensajes de otros user
{
    Message sms;
    std::string mensaje;

    //TODO:leer mensaje que llega
    sslSocket->read(const_cast<char*>(mensaje.c_str()),qstrlen(mensaje.c_str()));

    //TODO2:deserializar
    sms.ParseFromString(mensaje);

    //TODO3:mostrar mensaje por pantalla

    //TODO4:Ir a la carpeta con avatares y mostrarlo
    //TODO5:envio de imagen Qbuffer
}


