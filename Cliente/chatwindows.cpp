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

    connect(mySocket->sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    if(!isConnected){
       ui->plainTextEditrecive->setDisabled(true);
       ui->lineEditTexTenv->setDisabled(true);
    }

    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
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
    message.set_ip("");
    message.set_port(0);
    message.set_type(4);
    message.set_salaname(namesala.toStdString());

    //SERIALIZAMOS LA INFO
    mensaje_envio = message.SerializeAsString();
    qDebug() << QString::fromStdString(mensaje_envio);
    mySocket->sslSocket->write(mensaje_envio.c_str(), mensaje_envio.length());
    mySocket->sslSocket->waitForBytesWritten();
    mySocket->sslSocket->disconnect();

    this->hide();
    VentanaPrincipal principalwindows;
    principalwindows.exec();
}


void ChatWindows::on_pushButtonSalir_clicked()
{

    QString mensaje;
    Message message;
    std::string mensaje_envio;

    mensaje = ui->lineEditTexTenv->text();
    message.set_username(mySocket->username.toStdString());
    message.set_ip("");
    message.set_port(0);
    message.set_type(4);
    message.set_salaname(namesala.toStdString());

     //SERIALIZAMOS LA INFO
     mensaje_envio = message.SerializeAsString();
     qDebug() << QString::fromStdString(mensaje_envio);
     mySocket->sslSocket->write(mensaje_envio.c_str(), mensaje_envio.length());
     mySocket->sslSocket->waitForBytesWritten();
     mySocket->sslSocket->disconnect();

     qApp->exit();


}

void ChatWindows::on_lineEditTexTenv_returnPressed()
{

    if(!ui->lineEditTexTenv->text().isEmpty()){
        QString mensaje;
        Message message;
        std::string mensaje_envio;

        //Creacion del paquete
        mensaje = ui->lineEditTexTenv->text();
        message.set_username(mySocket->username.toStdString());
        message.set_ip(mySocket->my_ip.toString().toStdString());
        message.set_port(mySocket->my_port);
        message.set_message(mensaje.toStdString().c_str());
        message.set_salaname(namesala.toStdString());
        message.set_type(2);//Para saber el tipo del paquete
        //message.set_avatar();
        //TODO3:Falta envio con avatares.message.set_avatar()

        //SERIALIZAMOS LA INFO
        mensaje_envio = message.SerializeAsString();

        //ENVIO AL SERVIDOR
        mySocket->sslSocket->write(mensaje_envio.c_str(), mensaje_envio.length());
        ui->plainTextEditrecive->appendPlainText(ui->lineEditTexTenv->text());
        ui->lineEditTexTenv->setText("");

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

     //ENVIO AL SERVIDOR
     mySocket->sslSocket->write(mensaje_envio.c_str(), mensaje_envio.length());
     ui->lineEditTexTenv->setEnabled(true);
     isConnected=true;
    }

}

void ChatWindows::readyRead(){//para leer los mensajes que me enviar el servidor

        mySocket->sslSocket->readAll(); //Si leemos del servidor
        Message message;
        std::string mensaje;
        std::string cout_mensaje;
        //tipo_var cout_avatar;

        //DESCERIALIZACION -->guardamos en mensaje lo que tenga message que lo enviar el server
        message.ParseFromString(mensaje);
        cout_mensaje = message.message();
        //TODO:almacenar avatar y mostrar.
        //cout_avatar = meesage.avatar()...

        ui->plainTextEditrecive->appendPlainText(QString::fromStdString(cout_mensaje));
        //ui->plainTextEditrecive->appendPlainText(cout_avatar);

void ChatWindows::readyRead()
{
    qDebug() << "Entrando mensaje";

    Message sms;
    QByteArray mensaje;
    qDebug() << "Llega mensaje";

    //TODO:leer mensaje que llega

    mensaje = mySocket->sslSocket->readAll();
    qDebug() << mensaje;
    //TODO2:deserializar
    sms.ParseFromString(mensaje.toStdString()); //guardamos en mensaje lo que deserializamos en sms.

    if(sms.type() == 5){
        mySocket->logeado = true;
    }else if(sms.type() == 2){
        ui->plainTextEditrecive->appendPlainText(QString::fromStdString(sms.message()));
    }



}
