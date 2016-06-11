#include "perfilusers.h"
#include "ui_perfilusers.h"




PerfilUsers::PerfilUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerfilUsers)
{
    ui->setupUi(this);

    QSettings settings;

    QString username = getenv("USER");

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

            if(ui->lineEditNameuser->text().isEmpty())
                guardar.setValue("Name-User", getenv("USER"));
            else
                guardar.setValue("Name-User", ui->lineEditNameuser->text());

            if(ui->lineEditAvatar->text().isEmpty())
                guardar.setValue("Ruta_My_Avatar", "./Imagenes/haribo.jpg");
            else
                guardar.setValue("Ruta_My_Avatar", ui->lineEditAvatar->text());


            QDialog::done(r);
    }
    else
        QDialog::done(r);
}

void PerfilUsers::on_pushButtonSeleccion_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
    QSettings settings_imagen;

    if(!filename.isNull()){
        ui->lineEditAvatar->setText(filename);
    }
    else{
        ui->lineEditAvatar->setText("./Imagenes/haribo.jpeg");
    }
}

void PerfilUsers::on_pushButtonCamaraWeb_clicked()
{
    QSettings settings;
    VentanaCamera cam;
    //CameraWindow cam;
    cam.exec();

    ui->lineEditAvatar->setText(settings.value("Ruta_My_Avatar").toString());




}
