#include "loginuser.h"
#include "ui_loginuser.h"

LoginUser::LoginUser(QString ip_server, quint16 port_server ,QString sala, bool crear_sala, QString ruta_img, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginUser),
    ipserver_(ip_server),
    portserver_(port_server),
    namesala(sala),
    crearsala(crear_sala),
    ruta_img_user(ruta_img)
{
    ui->setupUi(this);

}

LoginUser::~LoginUser()
{
    delete ui;

}

void LoginUser::log()
{
    //qDebug() << "EMITE LOG";

    disconnect(chat, SIGNAL(logueado()), this, SLOT(log()));

    this->hide();
    chat->exec();

}

void LoginUser::noLog()
{
    QMessageBox::critical(NULL, "ERROR", "Usuario no registrado");
}

void LoginUser::on_pushButtonEntrar_clicked()
{


    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();
    mySocket = new My_Socket_Cliente(ipserver_, portserver_, username, password, ruta_img_user);
    chat = new ChatWindows(crearsala, namesala, mySocket);
    mySocket->sslSocket->waitForReadyRead(3000);



        connect(chat, SIGNAL(logueado()), this, SLOT(log()));
        connect(chat, SIGNAL(noLogueado()), this, SLOT(noLog()));

       if(mySocket->logeado == true ){
            this->hide();
            chat->exec();
       }



}
