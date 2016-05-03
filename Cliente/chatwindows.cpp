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
    qApp->quit();
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
        message.set_username(setting.value("Name-User").toString().toStdString());
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
    message.set_username(setting.value("Name-User").toString().toStdString());
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

     //ENVIO AL SERVIDOR
     mySocket->sslSocket->write(mensaje_envio.c_str(), qstrlen(mensaje_envio.c_str()));

     isConnected=true;
    }

}
