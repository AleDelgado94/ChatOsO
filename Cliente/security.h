#ifndef SECURITY_H
#define SECURITY_H

#include <QObject>

class Security : public QObject
{
    Q_OBJECT
public:
    explicit Security(QObject *parent = 0);

signals:

public slots:
};

#endif // SECURITY_H