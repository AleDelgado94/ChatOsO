#ifndef VENTANACAMERA_H
#define VENTANACAMERA_H

#include <QDialog>
#include <QList>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QCameraInfo>
#include <QWidget>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QSettings>
#include <QString>

#include "capturebuffer.h"
#include "perfilusers.h"

namespace Ui {
class VentanaCamera;
}

class VentanaCamera : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaCamera(QWidget *parent = 0);
    ~VentanaCamera();

private slots:


    void on_pushButtonCapturar_clicked();

    void on_pushButtonCancelar_clicked();

private:
    Ui::VentanaCamera *ui;
    QCameraImageCapture* imageCapture_;
    QCamera* camera;
    QCameraViewfinder* viewfinder;
};

#endif // VENTANACAMERA_H
