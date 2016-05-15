#ifndef LOGINUSER_H
#define LOGINUSER_H

#include <iostream>
#include <QMainWindow>
#include <QDialog>
#include <QtSql>
#include <QMessageBox>


#include "chatwindows.h"
#include "my_socket_cliente.h"

namespace Ui {
class LoginUser;
}

class LoginUser : public QDialog
{
    Q_OBJECT

public:
    explicit LoginUser(QString ip_server, quint16 port_server, QString sala, bool crear_sala, QWidget *parent = 0);
    ~LoginUser();

private slots:
    void on_pushButtonEntrar_clicked();

private:
    Ui::LoginUser *ui;
    My_Socket_Cliente *mySocket;
    QString ipserver_;
    quint16 portserver_;
    QString namesala;
    bool crearsala;
};

#endif // LOGINUSER_H
