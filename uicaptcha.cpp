#include "uicaptcha.h"
#include "ui_uicaptcha.h"

UICaptcha::UICaptcha(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UICaptcha)
{
    ui->setupUi(this);
}

UICaptcha::~UICaptcha()
{
    delete ui;
}
