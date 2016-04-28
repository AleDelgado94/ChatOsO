#include "login.h"
#include "ui_login.h"
#include "my_socket_cliente.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButtonEntrar_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();
    My_Socket_Cliente *mySslSockey = My_Socket_Cliente();

    if(usuario == username && usuario != "" ){

        this->hide();

        QDialog q;
        q.exec();
    }
    else{
        QMessageBox::critical(NULL, "Error", "Usuario no registrado");
    }

}
