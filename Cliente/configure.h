#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QDialog>

namespace Ui {
class Configure;
}

class Configure : public QDialog
{
    Q_OBJECT

public:
    explicit Configure(QWidget *parent = 0);
    ~Configure();
public slots:
    void done(int r);

private:
    Ui::Configure *ui;
};

#endif // CONFIGURE_H
