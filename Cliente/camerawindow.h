#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QDialog>
#include <QCamera>
#include <QCameraViewfinder>

namespace Ui {
class CameraWindow;
}

class CameraWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CameraWindow(QWidget *parent = 0);
    ~CameraWindow();

private:
    Ui::CameraWindow *ui;
    QCameraViewfinder *viewfinder;
    QCamera *camera;
};

#endif // CAMERAWINDOW_H
