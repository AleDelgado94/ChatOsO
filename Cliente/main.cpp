#include <QHostAddress>
#include<QHostInfo>
#include <QApplication>

#include "login.h"
#include "ventanaprincipal.h"
<<<<<<< HEAD
#include "my_socket_cliente.h"
//#include "protomessage.pb.h"
=======
#include "chatwindows.h"
#include <QApplication>
#include "protomessage.pb.h"
>>>>>>> ChatWindos

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Universidad de La Laguna");
    QApplication::setApplicationName("ChatOsO");
    QApplication::setOrganizationDomain("ull.es");
    //Login w;
    //w.show();
<<<<<<< HEAD
    //VentanaPrincipal v;
    //v.show();

        QList<QHostAddress> list = QNetworkInterface::allAddresses();
=======
   VentanaPrincipal v;
   v.show();
    //ChatWindows chat;
    //chat.show();
>>>>>>> ChatWindos

        for( int i = 0; i< list.size(); i++){
           qDebug() << list.at(i).toString();
        }

    return a.exec();
}
