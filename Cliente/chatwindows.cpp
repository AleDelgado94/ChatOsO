#include "chatwindows.h"
#include "ui_chatwindows.h"

ChatWindows::ChatWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindows),
    proceso_(this)

{
    ui->setupUi(this);

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
        mensaje = ui->lineEditTexTenv->text();
        //TODO:Crear estructura mesanje y usuario y socket
        message.set_username();
        //TODO2:crear salas y envio con la salamessage.set_salaname()
        message.set_ip(My_Socket_Cliente::my_ip);
        message.set_port(My_Socket_Cliente::my_port);
        message.set_message(mensaje.toStdString().c_str());
        //TODO3:Falta envio con avatares.message.set_avatar()

        //SERIALIZAMOS LA INFO
        mensaje_envio = message.SerializeAsString();

        //ENVIO AL SERVIDOR
        My_Socket_Cliente::sslSocket->write(mensaje_envio.c_str(), qstrlen(mensaje_envio.c_str()));

    }
    else{
        QMessageBox::critical(NULL, "Error", "Campo de mensaje vacio");
    }

}
