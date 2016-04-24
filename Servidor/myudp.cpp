#include "myudp.h"

MyUDP::MyUDP(QString dirServer, quint16 portServer, QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress(dirServer), portServer);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void MyUDP::send(Message packet)
{
    std::string p;
    quint16 port = packet.port();
    QString address = QString::fromStdString(packet.ip());
    /*QString username = QString::fromStdString(packet.username());
    QString sala = QString::fromStdString(packet.salaname());
    uint32_t tipo = packet.type();
    QString message = QString::fromStdString(packet.message());

    QString paquete = QString::fromStdString(packet.SerializeAsString());
    QByteArray Buffer;
    Buffer.append(paquete);

    std::cout << port << address.toStdString() << username.toStdString() <<
              sala.toStdString() << tipo << message.toStdString() << std::endl;*/

    p = packet.SerializeAsString();

    socket->connectToHost(QHostAddress(address), quint16(port));
    socket->write(p.c_str(), qstrlen(p.c_str()));

}

void MyUDP::readyRead()
{

    while(socket->hasPendingDatagrams()){

        QByteArray Buffer;
        Buffer.resize(socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);

        Message m;
        m.ParseFromString(Buffer.toStdString());

        qDebug() << "Tipo: " << m.type();
        qDebug() << "Message form: " << m.ip().c_str();
        qDebug() << "Usuario: " << m.username().c_str();
        qDebug() << "Message port: " << m.port();
        qDebug() << "Sala: " << m.salaname().c_str();
        qDebug() << "Message: " << m.message().c_str();




        switch(m.type()){
        case 0: //CREAR LA SALA
        {
            QSqlDatabase *db = new QSqlDatabase("QSQLITE");
            db->setDatabaseName("database.sqlite");
            QSqlQuery query(*db);

            int port = m.port();
            std::string puerto;
            puerto = std::to_string(port);

            //FALTA COMPROBAR SI YA ESTÁ CREADA LA TABLA
            std::string crear_tabla = "CREATE TABLE " + m.salaname() + " (usuario VARCHAR(50) NOT NULL, puerto INT NOT NULL, direccion VARCHAR(50) PRIMARY KEY NOT NULL);";
            std::string crear_sala = "CREATE TABLE MESSAGE_" + m.salaname() + " (id INT PRIMARY KEY AUTOINCREMENT, usuario VARCHAR(50) NOT NULL, mensaje TEXT, foto TEXT);";
            std::string insertar_usuario = "INSERT INTO " + m.salaname() + " (usuario, puerto, direccion) VALUES (" + m.username() + ", " + puerto + ", " + m.ip() + ")";

            query.exec(QString::fromStdString(crear_tabla));
            query.exec(QString::fromStdString(crear_sala));
            query.exec(QString::fromStdString(insertar_usuario));


        }
            break;
        case 1: //UNIRSE A LA SALA

            break;
        case 2: //ENVIO DE MENSAJE

            break;
        case 3: //RECEPCION DE MENSAJE

            break;
        default: break;
        }



    }


}
