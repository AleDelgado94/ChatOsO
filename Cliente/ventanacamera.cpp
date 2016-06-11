#include "ventanacamera.h"
#include "ui_ventanacamera.h"

VentanaCamera::VentanaCamera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaCamera)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("/usr/share/icons/hicolor/32x32/apps/ChatOsO.png"));

    QCameraInfo camerasInfo("/dev/video0");
    camera = new QCamera(camerasInfo, this);//Creamos un objeto QCamera
    viewfinder = new QCameraViewfinder(this);
    ui->verticalLayout->addWidget(viewfinder);

  //  viewfinder->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    camera->setViewfinder(viewfinder);
    camera->setCaptureMode(QCamera::CaptureStillImage);

    camera->start();

    auto mensaje = QMessageBox::information(this, "Capturar", "Apretar Enter o botón Capturar para capturar la imagen");


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
    ruta_save += "/.local/ChatOsO/Images/foto.jpg";

    QImageEncoderSettings imageSettings;

    imageCapture_ = new QCameraImageCapture(camera);

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

