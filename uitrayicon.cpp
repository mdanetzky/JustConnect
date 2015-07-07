#include "uitrayicon.h"

UITrayIcon::UITrayIcon()
{
}

void UITrayIcon::showMessage(QString msg)
{
    init();
    if(trayIcon != NULL){
        trayIcon->showMessage("JustConnect", msg);
    }
}

void UITrayIcon::setIcon(QString iconFile)
{
    init();
    if(trayIcon != NULL){
        trayIcon->setIcon(QIcon(iconFile));
        trayIcon->show();
    }
}

void UITrayIcon::remove()
{
    if(trayIcon != NULL){
        trayIcon->hide();
        delete trayIcon;
        trayIcon = NULL;
    }
}

void UITrayIcon::connectIcon(QObject *context, const char* slot)
{
    init();
    if(trayIcon != NULL){
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                context, slot);

    }
}

void UITrayIcon::init()
{
    if(!initialized){
        initialized = true;
        trayIcon = NULL;
        if(QSystemTrayIcon::isSystemTrayAvailable()){
            trayIcon = new QSystemTrayIcon();
        }
    }
}
QSystemTrayIcon *UITrayIcon::trayIcon;
bool UITrayIcon::initialized;
