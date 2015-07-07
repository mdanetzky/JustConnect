#ifndef UITRAYICON_H
#define UITRAYICON_H

#include <QObject>
#include <QString>
#include <QSystemTrayIcon>
#include <QIcon>

class UITrayIcon: public QObject
{
    Q_OBJECT
public:
    UITrayIcon();
    static void showMessage(QString msg);
    static void setIcon(QString iconFile);
    static void remove();
    static void connectIcon(QObject *context, const char *slot);
private:
    static QSystemTrayIcon *trayIcon;
    static void init();
    static bool initialized;
};

#endif // UITRAYICON_H
