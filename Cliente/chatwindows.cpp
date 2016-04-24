#include "chatwindows.h"
#include "ui_chatwindows.h"
#include "ventanaprincipal.h"

ChatWindows::ChatWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindows)
{
    ui->setupUi(this);
}

ChatWindows::~ChatWindows()
{
    delete ui;
}

void ChatWindows::on_pushButtonDesconectar_clicked()
{
    qApp->quit();
}


void ChatWindows::on_pushButtonSalir_clicked()
{
    this->hide();

    VentanaPrincipal principalwindows;
    principalwindows.exec();
}
