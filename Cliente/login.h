#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <QMainWindow>
#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QProcess>

#include "my_socket_cliente.h"


namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_pushButtonEntrar_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
