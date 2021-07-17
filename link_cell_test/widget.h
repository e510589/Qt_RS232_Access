#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "link_data.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    link_data *ld;
    QThread  t;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void serial_data_in(QString);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;

signals:
    void startwork();
};

#endif // WIDGET_H
