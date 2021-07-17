#ifndef LINK_DATA_H
#define LINK_DATA_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QDebug>
#include <QSerialPort>
#include <QString>
#include <QCoreApplication>

class link_data : public QObject
{
    Q_OBJECT

    QByteArray readBuffer;
    QByteArray restBuffer;

public:
    explicit link_data(QObject *parent = 0);

    QSerialPort *serialport;

    bool OpenLink();

    void CloseLink();

    bool IsOpen();

    int Write(QByteArray);

    bool canrun;

private:

    bool is_open;



    QByteArray bufferhandler(QByteArray);

signals:

    void indicated_ui(QString);

public slots:

    void readyRead();

    void readyRead_s();


};

#endif // LINK_DATA_H
