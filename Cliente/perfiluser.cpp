#include "perfiluser.h"
#include "ui_perfiluser.h"

PerfilUser::PerfilUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerfilUser)
{
    ui->setupUi(this);

    QSettings settings;

    QByteArray username = qgetenv("USER");
    if(!username.isNull())
       ui->lineEditNameuser->setText(settings.value("Name-User", username).toString());
    else
       ui->lineEditNameuser->setText(settings.value("Name-User", "").toString());
}

PerfilUser::~PerfilUser()
{
    delete ui;
}

void PerfilUser::done(int r)
{
    if(r == QDialog::Accepted){
        if(ui->lineEditNameuser->text().isEmpty()){
          QMessageBox::critical(this,"ChatOsO", "El campo de nombre de usuario debe estar lleno");
          return;
        }
        else{
            QSettings guardar;
            if(ui->lineEditNameuser->text().isEmpty())
                guardar.remove("Name-User");
            else
                guardar.setValue("Name-User", ui->lineEditNameuser->text());

            QDialog::done(r);
        }
     else
        QDialog::done(r);

}

void PerfilUser::on_pushButtonSeleccion_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
}
