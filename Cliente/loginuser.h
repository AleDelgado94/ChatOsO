#ifndef LOGINUSER_H
#define LOGINUSER_H


#include <iostream>
#include <QMainWindow>
#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include "my_socket_cliente.h"
#include "chatwindows.h"


namespace Ui {
class LoginUser;
}

class ChatWindows;

class LoginUser : public QDialog
{
    Q_OBJECT

public:
    explicit LoginUser(QString ip_server, quint16 port_server, QString sala, bool crear_sala, QString ruta_img, QWidget *parent = 0);
    ~LoginUser();

private slots:
    void on_pushButtonEntrar_clicked();

public slots:
    void log();
    void noLog();


private:
    Ui::LoginUser *ui;
    My_Socket_Cliente *mySocket;
    ChatWindows *chat;
    QString ipserver_;
    quint16 portserver_;
    QString namesala;
    bool crearsala;
    QString ruta_img_user;
};

#endif // LOGINUSER_H
