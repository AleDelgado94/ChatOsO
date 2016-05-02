#ifndef CHATWINDOWS_H
#define CHATWINDOWS_H

#include "my_socket_cliente.h"
#include "ventanaprincipal.h"

#include <QDialog>
#include <QProcess>
#include <QMessageBox>

namespace Ui {
class ChatWindows;
}

class ChatWindows : public QDialog
{
    Q_OBJECT

private:
    QString username;

public:
    explicit ChatWindows(QWidget *parent = 0);
    ~ChatWindows();


private slots:
    void on_pushButtonDesconectar_clicked();

    void on_pushButtonSalir_clicked();

    void on_lineEditTexTenv_returnPressed();

private:
    Ui::ChatWindows *ui;
    QProcess proceso_;
};

#endif // CHATWINDOWS_H
