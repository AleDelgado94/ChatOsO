#include "login.h"
#include "ventanaprincipal.h"
#include <QApplication>

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


    return a.exec();
}
