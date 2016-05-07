#ifndef PERFILUSER_H
#define PERFILUSER_H

#include <QDialog>
#include <QFileDialog>
#include <QtGlobal>
#include <QMessageBox>
#include <QSettings>


namespace Ui {
class PerfilUser;
}

class PerfilUser : public QDialog
{
    Q_OBJECT

public:
    explicit PerfilUser(QWidget *parent = 0);
    ~PerfilUser();

public slots:
    void done(int r);

private slots:
    void on_pushButtonSeleccion_clicked();

private:
    Ui::PerfilUser *ui;
};

#endif // PERFILUSER_H
