
#include <QCoreApplication>
#include <QSettings>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QSslSocket>
#include <QUdpSocket>
#include <fstream>
#include <sstream>
#include <sys/socket.h>

#include "protomessage.pb.h"
#include "myudp.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationName("ChatOsO");
    QCoreApplication::setOrganizationDomain("ChatOsO");
    QCoreApplication::setApplicationName("ServerChatOsO");

    QSettings settings;



    int op;
    std::string ip_option;
    std::string port_option;

    if(argc == 1){

        ip_option = settings.value("Direccion", "127.0.0.1").toString().toStdString();
        port_option = settings.value("Puerto", 3000).toString().toStdString();




    }else{

        while( (op = getopt(argc, argv, "s:p:01")) != -1){
            switch (op){
                case '0':
                case '1': break;
                case 's': ip_option = std::string(optarg); break;
                case 'p': port_option = std::string(optarg); break;
                case '?': break;
                default: std::fprintf(stderr, "?? getopt devolvio codigo de error 0%o ??\n", op);

            }
        }
    }

    QString port = QString::fromStdString(port_option);
    quint16 portServer = port.toUShort();

    MyUDP client(QString("127.0.0.10"), quint16(8005));
    MyUDP server(QString::fromStdString(ip_option), portServer);

    //std::cout << ip_option << " " << port_option << std::endl;

    Message m;
    m.set_type(2);
    m.set_ip("127.0.0.2");
    m.set_port(8003);
    m.set_message("Hola me llamo alejandro");
    m.set_salaname("SalaJuan");
    m.set_username("PEPE");

    //std::string paquete;
    //paquete = m.SerializeAsString();

    client.send(m);


    //client.socket->connectToHost(QHostAddress(QString("127.0.0.2")), 8003);
    //client.socket->write(paquete.c_str(), qstrlen(paquete.c_str()));




    return a.exec();
}
