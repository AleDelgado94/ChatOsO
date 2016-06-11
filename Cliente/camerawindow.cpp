#include "camerawindow.h"
#include "ui_camerawindow.h"

#include <QPushButton>
#include <QDebug>

CameraWindow::CameraWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraWindow)
{
    ui->setupUi(this);

    auto button = new QPushButton("Capturar", this);

    camera = new QCamera("/dev/video0", this);
    viewfinder = new QCameraViewfinder(this);
    ui->verticalLayout_2->addWidget(viewfinder);
    ui->verticalLayout_2->addWidget(button);


    camera->setViewfinder(viewfinder);
    camera->setCaptureMode(QCamera::CaptureStillImage);

    connect(camera, static_cast<void(QCamera::*)(QCamera::Error)>(&QCamera::error), [=](QCamera::Error value){
        qDebug() << value;
    });

    camera->start();


}

CameraWindow::~CameraWindow()
{
    delete ui;
}
