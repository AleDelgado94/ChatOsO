#ifndef PERFIL_H
#define PERFIL_H

#include <QDialog>

namespace Ui {
class Perfil;
}

class Perfil : public QDialog
{
    Q_OBJECT

public:
    explicit Perfil(QWidget *parent = 0);
    ~Perfil();
public slots:
    void done(int r);

private slots:
    void on_pushButtonSeleccion_clicked();

private:
    Ui::Perfil *ui;
};

#endif // PERFIL_H
