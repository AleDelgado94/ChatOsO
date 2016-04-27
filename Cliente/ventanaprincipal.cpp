#include <QSettings>

#include "ventanaprincipal.h"
#include "perfil.h"
#include "configure.h"
#include "ui_ventanaprincipal.h"
#include "chatwindows.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaPrincipal),
    isConnected_(false)
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

void VentanaPrincipal::on_lineEditConectsalas_textEdited()
{
    ui->lineEditCrearsalas->setDisabled(true);
    ui->labelCrearSalas->setDisabled(true);

        if(ui->lineEditConectsalas->text().isEmpty()){
            ui->labelCrearSalas->setDisabled(false);
            ui->lineEditCrearsalas->setDisabled(false);
        }
}

void VentanaPrincipal::on_lineEditCrearsalas_textEdited()
{
    ui->labelConectsalas->setDisabled(true);
    ui->lineEditConectsalas->setDisabled(true);

        if(ui->lineEditCrearsalas->text().isEmpty()){
            ui->labelConectsalas->setDisabled(false);
            ui->lineEditConectsalas->setDisabled(false);
        }
}

void VentanaPrincipal::on_pushButtonConectar_clicked()
{
    isConnected_=true;
    this->hide();
    ChatWindows chat;
    chat.exec();

    if(isConnected_){
        //TODO:Crear socket y enviar primera estructura
    }
}
