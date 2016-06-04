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


    QString titulo("ChatOsO - ");
    titulo += mySocket->username;
    this->setWindowTitle(titulo);
    qDebug() << this->windowTitle();

    connect(mySocket->sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(rejected()), this, SLOT(cerrando()));

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
                  //Teniendo el tamaño de paquete lo leemos del buffer
              } if ((tamPacket !=0) && (mySocket->sslSocket->bytesAvailable() >=tamPacket )){
                 buffer=mySocket->sslSocket->read(tamPacket);
                 paquete.ParseFromString(buffer.toStdString());
                 tamPacket =0;
                 return paquete;

             }else{
                  paquete.set_type(10);
                  return paquete;
              }

    }
    paquete.set_type(10);
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
        pressed = "<p style=font-size:15px>" + ui->lineEditTexTenv->text() + "</p>";
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

    while(mySocket->sslSocket->bytesAvailable()){


        Message sms;
        sms = deserializar();


        if(sms.type() == 10){
            return;
        }


        if(sms.type() == 5){
            QBuffer* buffer = new QBuffer;
            buffer->open(QIODevice::ReadWrite);
            QByteArray b;
            quint64 bytes = buffer->write(QByteArray::fromBase64(sms.avatar().data()));

            buffer->seek(buffer->pos() - bytes);
            QImage image;
            qDebug() << image.loadFromData(buffer->buffer(), "JPG");



            QString ruta("/usr/local/share/ChatOsO/Usuarios/"+ mySocket->username+"/");

            std::string r = ruta.toStdString();
             mkdir(r.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

            ruta +=  QString::fromStdString(sms.username());
            qDebug() << ruta;
            QImageWriter img(ruta, "jpg");

            img.write(image);

        }else if(sms.type() == 2){

            qDebug() << "Entrando mensaje de otro usuario";

            QString ruta("/usr/local/share/ChatOsO/Usuarios/"+ mySocket->username + "/");
            QString imagen;
            QString mensaje;
            imagen = "<img width='30' height='30' src='"+ ruta + QString::fromStdString(sms.username()) + ".jpg'>";
            mensaje = "<span style=font-size:15px>" + QString::fromStdString(sms.message()) + "</span>";
            //TODO: Color y size(style=font-size:) letras como en html
            ui->textEditReceive->append(imagen + "    " + mensaje);
            ui->textEditReceive->setAlignment(Qt::AlignLeft);
        }
        else if(sms.type() == 11){
            //EMITIR SEÑAL DE LOGUEADO
            mySocket->logeado = true;
            //mySocket->sslSocket->waitForReadyRead(3000);
            emit logueado();



        }else if(sms.type() == 12){
            //EMITIR SEÑAL DE NO LOGUEADO
            mySocket->logeado = false;
        }
        else
            return;
    }
}

void ChatWindows::cerrando()
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
