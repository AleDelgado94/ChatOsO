#include "ventanacamera.h"
#include "ui_ventanacamera.h"

VentanaCamera::VentanaCamera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaCamera)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("/usr/share/icons/hicolor/32x32/apps/ChatOsO.png"));

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &camerasInfo, cameras) {
        if(camerasInfo.deviceName() == "/dev/video0"){
            camera = new QCamera(camerasInfo);//Creamos un objeto QCamera
            viewfinder = new QCameraViewfinder();

            camera->setViewfinder(viewfinder);
            camera->setCaptureMode(QCamera::CaptureViewfinder);
            //camera->setCaptureMode(QCamera::CaptureStillImage);
            ui->verticalLayout_2->addWidget(viewfinder);
            //ui->verticalLayoutCamera->addWidget(viewfinder);
            viewfinder->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
            viewfinder->show();

            camera->start();

        }
    }
}

VentanaCamera::~VentanaCamera()
{
    delete ui;
    delete viewfinder;
    delete camera;
}


void VentanaCamera::on_pushButtonCapturar_clicked()
{


    QSettings setting;
    QString home = getenv("HOME");
    QString ruta_save(home);
    ruta_save += "/.local/ChatOsO/Images/foto.jpeg";

    QImageEncoderSettings* imageSettings = new QImageEncoderSettings();

    imageSettings->setCodec("image/jpeg");
    imageSettings->setResolution(160, 120);
    imageSettings->setQuality(QMultimedia::VeryLowQuality);

    imageCapture_ = new QCameraImageCapture(camera);
    imageCapture_->setEncodingSettings(*imageSettings);
    imageCapture_->capture(ruta_save);


    setting.setValue("Ruta_My_Avatar", ruta_save);



    this->reject();
}

void VentanaCamera::on_pushButtonCancelar_clicked()
{
    this->reject();
}
