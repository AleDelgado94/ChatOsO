#include <QFileDialog>
#include <QtGlobal>
#include <QMessageBox>
#include <QSettings>

#include "configure.h"
#include "ui_configure.h"

Configure::Configure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configure)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("/usr/share/icons/hicolor/32x32/apps/ChatOsO.png"));

    QSettings settings;
    ui->lineEditDirServer->setText(settings.value("Dir-Server","127.0.0.1").toString());
    ui->spinBoxPort->setValue(settings.value("Puerto","1025").toInt());
}

Configure::~Configure()
{
    delete ui;
}

void Configure::done(int r)
{
    if(r == QDialog::Accepted){
        if(ui->lineEditDirServer->text().isEmpty()){
            QMessageBox::critical(this,"ChatOsO", "El campo direccion del servidor debe estar lleno");
            return;
        }
        else{
            QSettings guardar;
            guardar.setValue("Dir-Server", ui->lineEditDirServer->text());
            guardar.setValue("Puerto",ui->spinBoxPort->value());
        }
    QDialog::done(r);
    }
    else{
        QDialog::done(r);

    }

}


