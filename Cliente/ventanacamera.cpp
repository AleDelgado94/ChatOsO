#include "ventanacamera.h"
#include "ui_ventanacamera.h"

VentanaCamera::VentanaCamera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaCamera)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("/usr/share/icons/hicolor/32x32/apps/ChatOsO.png"));

    QPushButton* button = new QPushButton("hola", this);
    ui->verticalLayout->addWidget(button);

    QCameraInfo camerasInfo("/dev/video0");
    camera = new QCamera(camerasInfo, this);//Creamos un objeto QCamera
    viewfinder = new QCameraViewfinder(this);
    ui->verticalLayout->addWidget(viewfinder);

  //  viewfinder->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    camera->setViewfinder(viewfinder);
    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->start();
}

VentanaCamera::~VentanaCamera()
{
    delete ui;
}


void VentanaCamera::on_pushButtonCapturar_clicked()
{


    QSettings setting;
    QString home = getenv("HOME");
    QString ruta_save(home);
    ruta_save += "/.local/ChatOsO/Images/foto.jpeg";

    QImageEncoderSettings* imageSettings = new QImageEncoderSettings();

    imageSettings->setCodec("image/jpeg");
    imageSettings->setQuality(QMultimedia::VeryLowQuality);

    imageCapture_ = new QCameraImageCapture(camera);
    imageCapture_->setEncodingSettings(*imageSettings);
    imageCapture_->capture(ruta_save);

    connect(imageCapture_, SIGNAL(error(int,QCameraImageCapture::Error,QString)), this, SLOT(captureError(int,QCameraImageCapture::Error,QString)));


    setting.setValue("Ruta_My_Avatar", ruta_save);

    connect(imageCapture_, SIGNAL(imageSaved(int,QString)), this, SLOT(imagen_capturada()));

}

void VentanaCamera::captureError(int id ,QCameraImageCapture::Error error, QString errorString)
{
    qDebug() << errorString;
}

void VentanaCamera::imagen_capturada()
{
    this->reject();
}

void VentanaCamera::on_pushButtonCancelar_clicked()
{
    this->reject();
}
