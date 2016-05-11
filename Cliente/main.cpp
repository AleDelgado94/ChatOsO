#include <QHostAddress>
#include<QHostInfo>
#include <QApplication>

#include "loginuser.h"
#include "ventanaprincipal.h"
#include "my_socket_cliente.h"
#include "protomessage.pb.h"
#include "protomessage.pb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Universidad de La Laguna");
    QApplication::setApplicationName("ChatOsO");
    QApplication::setOrganizationDomain("ull.es");
    //Login w;
    //w.show();

    VentanaPrincipal v;
    v.show();



    //ChatWindows chat();
    //chat.show();

    return a.exec();
}
