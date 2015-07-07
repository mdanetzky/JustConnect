#include "uidiagnostics.h"
#include "ui_uidiagnostics.h"
#include <QDebug>

UIDiagnostics::UIDiagnostics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UIDiagnostics)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(textChanged()));
}

UIDiagnostics::~UIDiagnostics()
{
    delete ui;
}

void UIDiagnostics::textChanged()
{
    QString text =  ui->lineEdit->text();
    qDebug() << "text entered "<<text;
    emit commandAT(text);
    ui->lineEdit->setText("");
}

void UIDiagnostics::modemAnswer(QString answer)
{
    qDebug() << "modem answer "<<answer;
    ui->textEdit->append(answer);
}
