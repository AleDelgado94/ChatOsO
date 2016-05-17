#include "chatwindows.h"
#include "ui_chatwindows.h"
#include <QImage>
#include <QPaintDevice>
#include <QLabel>
#include <QImageWriter>
#include <QPixmap>
#include <QBuffer>
#include <QTextEdit>
#include <QColor>
#include <QScrollBar>

ChatWindows::ChatWindows(bool crear_sala, QString name_sala, My_Socket_Cliente* socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindows),
    isConnected(false),
    crearsala_(crear_sala),
    namesala(name_sala),
    mySocket(socket),
    tamPacket(0)
{
    ui->setupUi(this);

    connect(mySocket->sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    if(!isConnected){
       ui->textEditReceive->setDisabled(true);
       ui->lineEditTexTenv->setDisabled(true);
    }


    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

ChatWindows::~ChatWindows()
{
    delete ui;
}

Message ChatWindows::deserializar()
{
    QByteArray buffer;
    Message paquete;

    while(mySocket->sslSocket->bytesAvailable() > 0){

             QDataStream in(mySocket->sslSocket);
             in.setVersion(QDataStream::Qt_4_0);
              if(mySocket->sslSocket->bytesAvailable() >= (int)(sizeof(qint32))&& (tamPacket==0))
              {
                  //Guardamos el tamaño del paquete
                  in >> tamPacket;
                  qDebug() << tamPacket;
                  //Teniendo el tamaño de paquete lo leemos del buffer
              } if ((tamPacket !=0) && (mySocket->sslSocket->bytesAvailable() >=tamPacket )){
                 buffer=mySocket->sslSocket->read(tamPacket);

                 qDebug() << buffer;

                 paquete.ParseFromString(buffer.toStdString());

                 tamPacket =0;
                 return paquete;

             }else{
                  qDebug() << "Llegan menos bytes";
                  paquete.set_type(10);
                  return paquete;
              }
                  //mySocket->sslSocket->readAll();

    }
    return paquete;
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

    QByteArray pkt(mensaje_envio.c_str(), mensaje_envio.size());
    //ENVIO del tamaño y paquete
    quint32 size_packet = pkt.size();
    QByteArray envio;
    QDataStream env(&envio, QIODevice::WriteOnly);
    env.setVersion(7);
    env << (quint32)size_packet;

    mySocket->sslSocket->write(envio);
    mySocket->sslSocket->write(pkt);
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

    QByteArray pkt(mensaje_envio.c_str(), mensaje_envio.size());
    //ENVIO del tamaño y paquete
    quint32 size_packet = pkt.size();
    QByteArray envio;
    QDataStream env(&envio, QIODevice::WriteOnly);
    env.setVersion(7);
    env << (quint32)size_packet;

    mySocket->sslSocket->write(envio);
    mySocket->sslSocket->write(pkt);
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

        QByteArray pkt(mensaje_envio.c_str(), mensaje_envio.size());
        //ENVIO del tamaño y paquete
        quint32 size_packet = pkt.size();
        QByteArray envio;
        QDataStream env(&envio, QIODevice::WriteOnly);
        env.setVersion(7);
        env << (quint32)size_packet;

        mySocket->sslSocket->write(envio);
        mySocket->sslSocket->write(pkt);
        mySocket->sslSocket->waitForBytesWritten();

        QString pressed;
        pressed = "<b>" + ui->lineEditTexTenv->text() + "</b>";

        ui->textEditReceive->append(pressed);
        ui->textEditReceive->setAlignment(Qt::AlignRight);
        ui->lineEditTexTenv->setText("");



        //ENVIO AL SERVIDOR
        //mySocket->sslSocket->write(mensaje_envio.c_str(), mensaje_envio.length());
        //ui->plainTextEditrecive->appendPlainText(ui->lineEditTexTenv->text());
        //ui->lineEditTexTenv->setText("");

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

        QByteArray pkt(mensaje_envio.c_str(), mensaje_envio.size());
        //ENVIO del tamaño y paquete
        quint32 size_packet = pkt.size();
        QByteArray envio;
        QDataStream env(&envio, QIODevice::WriteOnly);
        env.setVersion(7);
        env << (quint32)size_packet;

        //ENVIO AL SERVIDOR
        mySocket->sslSocket->write(envio);
        mySocket->sslSocket->write(pkt);
        mySocket->sslSocket->waitForBytesWritten();

         //mySocket->sslSocket->write(mensaje_envio.c_str(), mensaje_envio.length());
         ui->lineEditTexTenv->setEnabled(true);
         isConnected=true;
    }

}

void ChatWindows::readyRead()
{
    qDebug() << "Entrando mensaje";

    while(mySocket->sslSocket->bytesAvailable()){


        Message sms;
        sms = deserializar();


        if(sms.type() == 10){
            qDebug() << "Entra aqui";
            return;
        }


        if(sms.type() == 5){

            qDebug() << "Recibiendo imagenes";
            QBuffer* buffer = new QBuffer;
            buffer->open(QIODevice::ReadWrite);
            QByteArray b;
            quint64 bytes = buffer->write(QByteArray::fromBase64(sms.avatar().data()));

            buffer->seek(buffer->pos() - bytes);
            QImage image;
            qDebug() << image.loadFromData(buffer->buffer(), "JPG");


            QString ruta("../Cliente/Images/");
            ruta +=  QString::fromStdString(sms.username());
            qDebug() << ruta;
            QImageWriter img(ruta, "jpg");

            img.write(image);
            //avatar_->fromData(QByteArray::fromStdString(sms.avatar()), "JPG");
            //qDebug() << "El tamaño del avatar es de: " << avatar.size();

            mySocket->logeado = true;
        }else if(sms.type() == 2){
            QString mostrar;
            mostrar = "<img width='30' height='30' src='../Cliente/Images/" + QString::fromStdString(sms.username()) + ".jpg'>";
            //TODO: Color y Size letras mensaje
            ui->textEditReceive->append(mostrar + QString::fromStdString(sms.message()));
            ui->textEditReceive->setAlignment(Qt::AlignLeft);
        }
    }
}
