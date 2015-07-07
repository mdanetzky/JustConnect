#ifndef JUSTCONNECT_H
#define JUSTCONNECT_H

#include "uitrayicon.h"
#include "netmonitorworker.h"
#include "devicemodemworker.h"
#include "uidiagnostics.h"

#include <QObject>
#include <QHash>

class JustConnect: public QObject
{
    Q_OBJECT

public:
    JustConnect();
    ~JustConnect();
    void init();

public slots:
    void inetUp();
    void inetDown();
    void deviceFound(DeviceModem *modem);
    void modemRemoved(QString imei, QString name);
    void trayIconClick(QSystemTrayIcon::ActivationReason reason);
    void processKilled(QString name);
    void modemMessage(QString sourceImei, QString message);

private:
    NetMonitorWorker *netMonitorWorker;
    QHash<QString, DeviceModemWorker *> modemWorkersByImei;
    UIDiagnostics uiDiags;
};

#endif // JUSTCONNECT_H
