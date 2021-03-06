#ifndef PERFILUSERS_H
#define PERFILUSERS_H

#include <QDialog>
#include <QFileDialog>
#include <QtGlobal>
#include <QMessageBox>
#include <QSettings>

#include "ventanacamera.h"
#include "camerawindow.h"


namespace Ui {
class PerfilUsers;
}

class PerfilUsers : public QDialog
{
    Q_OBJECT

public:
    explicit PerfilUsers(QWidget *parent = 0);
    ~PerfilUsers();

public slots:
    void done(int r);

private slots:
    void on_pushButtonSeleccion_clicked();

    void on_pushButtonCamaraWeb_clicked();

    //void image_s(const QImage &imagen);



private:
    Ui::PerfilUsers *ui;
};

#endif // PERFILUSERS_H
