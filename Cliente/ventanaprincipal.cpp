#include <QSettings>

#include "ventanaprincipal.h"
#include "perfil.h"
#include "configure.h"
#include "ui_ventanaprincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

void VentanaPrincipal::on_pushButtonPerfil_clicked()
{
    Perfil perfil;
    perfil.exec();
}



void VentanaPrincipal::on_pushButtonConfig_clicked()
{
    Configure config;
    config.exec();
}

void VentanaPrincipal::on_pushButtonDesconectar_clicked()
{
    qApp->quit();
}
