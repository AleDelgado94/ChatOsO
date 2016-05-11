#include "chatwindows.h"
#include "ui_chatwindows.h"

ChatWindows::ChatWindows(bool crear_sala, QString name_sala, My_Socket_Cliente* socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindows),
    isConnected(false),
    crearsala_(crear_sala),
    namesala(name_sala),
    mySocket(socket)
{
    ui->setupUi(this);

    if(!isConnected){
       ui->plainTextEditrecive->setDisabled(true);
       ui->lineEditTexTenv->setDisabled(true);
    }
}

ChatWindows::~ChatWindows()
{
    delete ui;
}

void ChatWindows::on_pushButtonDesconectar_clicked()
{
    QString mensaje;
    Message message;
    std::string mensaje_envio;

    mensaje = ui->lineEditTexTenv->text();
    message.set_username(mySocket->username.toStdString());
    message.set_ip(mySocket->my_ip.toString().toStdString());
    message.set_port(mySocket->my_port);
    message.set_type(4);

     //SERIALIZAMOS LA INFO
     mensaje_envio = message.SerializeAsString();
     mySocket->sslSocket->write(mensaje_envio.c_str(), mensaje_envio.length());
     mySocket->sslSocket->waitForBytesWritten();
     mySocket->sslSocket->disconnect();
     qApp->exit();
}


void ChatWindows::on_pushButtonSalir_clicked()
{

    this->hide();

    VentanaPrincipal principalwindows;
    principalwindows.exec();
}

void ChatWindows::on_lineEditTexTenv_returnPressed()
{

    if(!ui->lineEditTexTenv->text().isEmpty()){
        QString mensaje;
        Message message;
        std::string mensaje_envio;
        QSettings setting;

        //Creacion del paquete
        mensaje = ui->lineEditTexTenv->text();
        message.set_username(mySocket->username.toStdString());
        message.set_ip(mySocket->my_ip.toString().toStdString());
        message.set_port(mySocket->my_port);
        message.set_message(mensaje.toStdString().c_str());
        message.set_salaname(namesala.toStdString());
        message.set_type(2);//Para saber el tipo del paquete

        //TODO3:Falta envio con avatares.message.set_avatar()

        //SERIALIZAMOS LA INFO
        mensaje_envio = message.SerializeAsString();

        //ENVIO AL SERVIDOR
        mySocket->sslSocket->write(mensaje_envio.c_str(), qstrlen(mensaje_envio.c_str()));
    }
    else{
        QMessageBox::critical(NULL, "Error", "Campo de mensaje vacio");
    }

}

void ChatWindows::on_pushButtonConectar_clicked()
{
    QString mensaje;
    Message message;
    std::string mensaje_envio;
    QSettings setting;
    mensaje = ui->lineEditTexTenv->text();
    message.set_username(mySocket->username.toStdString());
    message.set_ip(mySocket->my_ip.toString().toStdString());
    message.set_port(mySocket->my_port);
    message.set_message(mensaje.toStdString().c_str());
    message.set_salaname(namesala.toStdString());

    if(!isConnected){
        if(crearsala_ == true){ //crearsala_ = true me conecto a la sala
            message.set_type(1);//Para saber el tipo del paquete
        }
        else // crearsala_ = false estoy creando una sala
            message.set_type(0);//Para saber el tipo del paquete

     //SERIALIZAMOS LA INFO
     mensaje_envio = message.SerializeAsString();

     qDebug() << QString::fromStdString(mensaje_envio);
     qDebug() << mensaje_envio.size();
     qDebug() << mensaje_envio.length();
     qDebug() << QString::fromStdString(message.username());
     qDebug() << message.port();
     qDebug() << QString::fromStdString(message.message());
     qDebug() << QString::fromStdString(message.salaname());
     qDebug() << message.type();
     //qDebug() << qstrlen(QString::fromStdString(mensaje_envio.c_str()));

     //ENVIO AL SERVIDOR
     mySocket->sslSocket->write(mensaje_envio.c_str(), mensaje_envio.length());

     isConnected=true;
    }

}
