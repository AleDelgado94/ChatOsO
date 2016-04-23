#include "login.h"
#include <QApplication>
#include "protomessage.pb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();

    return a.exec();
}
