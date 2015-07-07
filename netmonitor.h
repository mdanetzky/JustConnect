#ifndef NETMONITOR_H
#define NETMONITOR_H
#include "netmonitorworker.h"
#include <QObject>
#include <QTcpSocket>

class NetMonitor: public QObject
{
    Q_OBJECT
public:
    NetMonitor();
    static bool testTcpReachable(QString address, int port);
    static NetMonitorWorker *startNetMonitoring();
};

#endif // NETMONITOR_H
