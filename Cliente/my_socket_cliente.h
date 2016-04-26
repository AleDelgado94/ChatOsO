#ifndef MY_SOCKET_CLIENTE_H
#define MY_SOCKET_CLIENTE_H

#include <QObject>

class My_Socket_Cliente : public QObject
{
    Q_OBJECT
public:
    explicit My_Socket_Cliente(QObject *parent = 0);

signals:

public slots:
};

#endif // MY_SOCKET_CLIENTE_H