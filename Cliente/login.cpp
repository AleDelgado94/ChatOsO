#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButtonEntrar_clicked()
{
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    QSqlDatabase *db = new QSqlDatabase("QSQLITE");
    db->setDatabaseName("database.sqlite");

    db->open();
    if(!db->open()){
        QMessageBox::about(NULL, "Usuario", "NO CONECTO");
        QMessageBox::critical(NULL, tr("Error"),
            tr("No se pudo conectar con la base de datos"));
    }


    QSqlQuery query(*db);
    QString consulta = "SELECT username FROM USUARIOS WHERE username='" +
                    username + "' AND password='" + password + "'";
    std::string c = consulta.toStdString();
    std::cout << c << std::endl;

    query.exec(consulta);
    QString usuario;
    while(query.next()){
        usuario = query.value(0).toString();
    }
    if(usuario == username && usuario != "" ){

        this->hide();

        QDialog q;
        q.exec();
    }
    else{
        QMessageBox::critical(NULL, "Error", "Usuario no registrado");
    }

}
