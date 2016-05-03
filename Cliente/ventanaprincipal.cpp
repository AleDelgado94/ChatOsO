#include "ventanaprincipal.h"
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

    bool conectarsala=false;
    QString nombre_sala;
    QSettings settings;

    if(!ui->lineEditConectsalas->text().isEmpty()){
       conectarsala=true;
    }
    if(conectarsala){
        nombre_sala = ui->lineEditConectsalas->text();
    }
    else
        nombre_sala = ui->lineEditCrearsalas->text();


    this->hide();
    LoginUser log(settings.value("Dir-Server").toString(), settings.value("Puerto").toInt(), nombre_sala,conectarsala);
    log.exec();

}
