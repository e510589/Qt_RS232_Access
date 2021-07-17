#include "link_data.h"

link_data::link_data(QObject *parent) : QObject(parent)
{
    serialport = new QSerialPort();
    is_open = false;

    connect(serialport,&QSerialPort::readyRead,this,&link_data::readyRead_s);
}

bool link_data::OpenLink()
{
    qDebug() << "Debug step 0";
    serialport->setPortName("ttyUSB0");
    serialport->setBaudRate(QSerialPort::Baud115200);
    serialport->setDataBits(QSerialPort::Data8);
    serialport->setStopBits(QSerialPort::OneStop);
    serialport->setParity(QSerialPort::NoParity);
    serialport->setFlowControl(QSerialPort::NoFlowControl);

    if(serialport->open(QIODevice::ReadWrite))
    {

        qDebug() << "Debug step 1";
        is_open = true;

        return true;

    }

    qDebug() << "Debug step 2";

    qDebug() << serialport->errorString();

    return false;
}

void link_data::CloseLink()
{
    serialport->close();
    is_open = false;
//    disconnect(serialport);
}

bool link_data::IsOpen()
{
    return is_open;
}

int link_data::Write(QByteArray data)
{
    int writelength = 0;

    if(serialport != NULL)
    {
        if(serialport->isOpen())
        {

            writelength = serialport->write(data);
        }
    }

    return writelength;
}


//Receive RS232 data from Serial Port by readyread signal

void link_data::readyRead_s()
{
    QByteArray byteread;
    QByteArray leopard_pdu;

    byteread = serialport->readAll();


    if(byteread.length() == 0)
    {
        if(restBuffer.length() == 0)
        {
            return;
        }
        else
        {
            leopard_pdu = bufferhandler(restBuffer);
        }
    }
    else
    {
        if(restBuffer.length()== 0)
        {

            leopard_pdu = bufferhandler(byteread);
        }
        else
        {

            restBuffer.append(byteread);

            leopard_pdu = bufferhandler(restBuffer);

        }

    }

    if(leopard_pdu != NULL)
    {
        QString pdu_string(leopard_pdu);
        emit indicated_ui(pdu_string);
    }

}

//Receive RS232 data from Serial Port by polling the RS232 by specifice thread

void link_data::readyRead()
{

    QByteArray byteread;
    QByteArray leopard_pdu;

    while(is_open)
    {
        QThread::msleep(10);


        byteread = serialport->readAll();


        if(byteread.length() == 0)
        {
            if(restBuffer.length() == 0)
            {
                continue;
            }
            else
            {

                leopard_pdu = bufferhandler(restBuffer);
            }
        }
        else
        {

            if(restBuffer.length()== 0)
            {

                leopard_pdu = bufferhandler(byteread);
            }
            else
            {

                restBuffer.append(byteread);

                leopard_pdu = bufferhandler(restBuffer); // data

            }

        }


        if(leopard_pdu != NULL)
        {
            QString pdu_string(leopard_pdu);
            emit indicated_ui(pdu_string);
        }
    }
}

QByteArray link_data::bufferhandler(QByteArray buffer)
{
    QByteArray pdu;
    restBuffer.resize(0);


    int index_som = -1;
    int index_eom = -1;

    if(buffer.length() < 6)
    {
        restBuffer.resize(buffer.length());

        for(int i = 0; i < restBuffer.length() ; i ++) restBuffer[i] = buffer[i];
        pdu.resize(0);
        return NULL;
    }

    for(int i = 0 ; i <buffer.length() -1 ; i++)
    {
        if((buffer.at(i) == 0x21) || (buffer.at(i) == 0x23))
        {
            index_som = i;
            break;
        }
    }

    if(index_som == -1)
    {
        restBuffer.resize(buffer.length());
        for(int i = 0; i < restBuffer.length() ; i ++) restBuffer[i] = buffer[i];
        pdu.resize(0);
        return NULL;
    }

    for(int i = 0; i< buffer.length() ; i++)
    {
        if(buffer.at(i) == 0x0D)
        {
            index_eom = i;
            break;
        }
    }

    if((index_eom <=index_som) || (index_eom == -1))
    {
        restBuffer.resize(buffer.length());
        for(int i = 0; i < restBuffer.length() ; i ++) restBuffer[i] = buffer[i];
        pdu.resize(0);
        return NULL;
    }

    int l = index_eom - index_som+1;

    pdu.resize(l);

    for(int i = 0 ; i < l; i ++) pdu[i] = buffer[index_som+i];

    if((buffer.length() -(pdu.length()+index_som)) > 0)
    {
        restBuffer.resize(buffer.length() - (pdu.length()+index_som));

        for(int i = 0; i < restBuffer.length() ; i ++) restBuffer[i] = buffer[index_eom+1+i];

    }



    return pdu;
}


