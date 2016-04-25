
#include <QCoreApplication>
#include <QSettings>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QSslSocket>
#include <QUdpSocket>
#include <fstream>
#include <QList>

#include "protomessage.pb.h"
#include "server.h"


QList<Client*> clients;

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

    quint16 port;
    port = QString::fromStdString(port_option).toUInt();

    Server servidor(QString::fromStdString(ip_option), quint16(port));



    return a.exec();
}
