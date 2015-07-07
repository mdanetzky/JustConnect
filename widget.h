#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::Widget *ui;
 QSystemTrayIcon *trayIcon;
};

#endif // WIDGET_H
