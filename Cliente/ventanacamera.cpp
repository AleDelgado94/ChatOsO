#include "ventanacamera.h"
#include "ui_ventanacamera.h"

VentanaCamera::VentanaCamera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaCamera)
{
    ui->setupUi(this);

    qDebug() << "entra";
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &camerasInfo, cameras) {
        if(camerasInfo.deviceName() == "/dev/video0"){
            camera = new QCamera(camerasInfo);//Creamos un objeto QCamera
            viewfinder = new QCameraViewfinder();
            ui->verticalLayoutCamera->addWidget(viewfinder);
            viewfinder->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
            viewfinder->show();
            camera->setViewfinder(viewfinder);
            camera->setCaptureMode(QCamera::CaptureStillImage);
            camera->setCaptureMode(QCamera::CaptureViewfinder);
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
    QString ruta_save = "/home/allbermc/.local/ChatOsO";
    imageCapture_ = new QCameraImageCapture(camera);
    imageCapture_->capture(ruta_save);
    //ruta_save +=//buscar algo que me devuelva el nombre de la imagen que genero
    setting.setValue("Ruta_My_Avatar", ruta_save);
}

void VentanaCamera::on_pushButtonCancelar_clicked()
{
    this->reject();
}
