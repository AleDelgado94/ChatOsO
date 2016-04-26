#include "login.h"
#include "ventanaprincipal.h"
#include "chatwindows.h"
#include <QApplication>
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
    //ChatWindows chat;
    //chat.show();


    return a.exec();
}
