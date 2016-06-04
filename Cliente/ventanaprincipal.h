#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QDialog>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#include <QIcon>

#include "perfilusers.h"
#include "configure.h"
#include "loginuser.h"


namespace Ui {
class VentanaPrincipal;
}

class VentanaPrincipal : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    ~VentanaPrincipal();

private slots:
    void on_pushButtonPerfil_clicked();

    void on_pushButtonConfig_clicked();

    void on_pushButtonDesconectar_clicked();

    void on_lineEditConectsalas_textEdited();

    void on_lineEditCrearsalas_textEdited();

    void on_pushButtonConectar_clicked();

private:
    Ui::VentanaPrincipal *ui;
    //My_Socket_Cliente *mySocket;
};

#endif // VENTANAPRINCIPAL_H
