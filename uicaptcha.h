#ifndef UICAPTCHA_H
#define UICAPTCHA_H

#include <QDialog>

namespace Ui {
class UICaptcha;
}

class UICaptcha : public QDialog
{
    Q_OBJECT

public:
    explicit UICaptcha(QWidget *parent = 0);
    ~UICaptcha();

private:
    Ui::UICaptcha *ui;
};

#endif // UICAPTCHA_H
