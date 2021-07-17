#include "widget.h"
#include "ui_widget.h"
#include <QHash>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ld = new link_data();
//    ld = NULL;

    connect(ld,&link_data::indicated_ui,this,&Widget::serial_data_in);

    QHash<int,link_data*> link_table;
    link_table.insert(0,new link_data());
}

Widget::~Widget()
{



    delete ui;
}

void Widget::serial_data_in(QString pdu)
{

    ui->textBrowser->append(pdu);

}

void Widget::on_pushButton_clicked()
{
    if(ui->textEdit->toPlainText().length() != 0)
    {

        QString cmd = ui->textEdit->toPlainText()+ "\r";
        QByteArray arr = cmd.toUtf8();

        if(ld != NULL)
        {
            ld->Write(arr);
        }
        else
        {
            ui->textBrowser->append("Seria port is closed.");
        }
    }

}

void Widget::on_pushButton_2_clicked()
{
    QByteArray cmd("!PCI,LOC,9B9E3423B8BED32CBA2517E32A61B41E433DDE81\r");

    if(ld != NULL)
    {
        ld->Write(cmd);
    }
    else
    {
        ui->textBrowser->append("Seria port is closed.");
    }

}

void Widget::on_pushButton_3_clicked()
{
//    ld = new link_data();
    if(ld->OpenLink())
    {
        ui->textBrowser->append("ttyUSB0 is open.");
    }
    else
    {
        ui->textBrowser->append("ttyUSB0 open failed.");
    }

//    ld->moveToThread(&t);
//    connect(&t,&QThread::started,ld,&link_data::readyRead);
//    connect(&t,&QThread::finished,ld,&QObject::deleteLater);
//    t.start();

}

void Widget::on_pushButton_4_clicked()
{
    ld->CloseLink();
//    ld->deleteLater();
//    ld = NULL;

//    t.quit();
}
