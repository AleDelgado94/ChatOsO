#include "perfilusers.h"
#include "ui_perfilusers.h"

PerfilUsers::PerfilUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerfilUsers)
{
    ui->setupUi(this);

    QSettings settings;

    QByteArray username = qgetenv("USER");

    if(!username.isNull())
       ui->lineEditNameuser->setText(settings.value("Name-User", username).toString());
    else
       ui->lineEditNameuser->setText(settings.value("Name-User", "").toString());
}

PerfilUsers::~PerfilUsers()
{
    delete ui;
}

void PerfilUsers::done(int r)
{
    QSettings guardar;

    if(r == QDialog::Accepted){
        if(ui->lineEditNameuser->text().isEmpty()){
          QMessageBox::critical(this,"ChatOsO", "El campo de nombre de usuario debe estar lleno");
          return;
        }
        else{
            if(ui->lineEditNameuser->text().isEmpty())
                guardar.setValue("Name-User", qgetenv("USER"));
            else
                guardar.setValue("Name-User", ui->lineEditNameuser->text());

            QDialog::done(r);
        }
        if(ui->lineEditAvatar->text().isEmpty()){
            QMessageBox::critical(this,"ChatOsO", "Debe seleccionar un avatar");
            return;
        }
        else{
            if(ui->lineEditAvatar->text().isEmpty())
                guardar.setValue("Ruta_My_Avatar", "./Imagenes/haribo.jpg");
            else
                guardar.setValue("Ruta_My_Avatar", ui->lineEditAvatar->text());

            QDialog::done(r);
        }
    }
    else
        QDialog::done(r);
}

void PerfilUser::on_pushButtonSeleccion_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
    QSettings settings_imagen;

    if(!filename.isNull()){
        ui->lineEditAvatar->setText(filename);
        ui->lineEditAvatar->setText(settings_imagen.value("Ruta_My_Avatar", filename).toString());
    }
}
