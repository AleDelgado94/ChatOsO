#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class VentanaPrincipal;
}

class VentanaPrincipal : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    ~VentanaPrincipal();

private slots:
    void on_pushButtonPerfil_clicked();

    void on_pushButtonConfig_clicked();

    void on_pushButtonDesconectar_clicked();

private:
    Ui::VentanaPrincipal *ui;
};

#endif // VENTANAPRINCIPAL_H
