#include "widget.h"
#include "ui_widget.h"
#include <QThread>
#include "uitrayicon.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}
void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        qDebug() << "Icon click";
        if(this->isVisible()){
            this->hide();
        } else {
            this->show();
        }
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "Icon dblclick";
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "Icon middle click";
        break;
    case QSystemTrayIcon::Context:
        qDebug() << "Icon right click";
        break;
    default:
        ;
    }
}

Widget::~Widget()
{
    delete ui;
}
