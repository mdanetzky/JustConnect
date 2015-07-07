#include "netmonitorworker.h"
#include "netmonitor.h"
#include <QThread>

NetMonitorWorker::NetMonitorWorker()
{
}

void NetMonitorWorker::process()
{
    bool currentlyInetReachable = true;
    int retries = 5;
    bool inetReachable;

    forever{
        QThread::msleep(1000);
        inetReachable = NetMonitor::testTcpReachable("8.8.8.8", 53);
        if(inetReachable != currentlyInetReachable){
            // Retry to check if it's not just a fluctuation
            if(!inetReachable){
                for(int retry = retries; retry; retry--){
                    QThread::msleep(300);
                    inetReachable = NetMonitor::testTcpReachable("8.8.8.8", 53);
                    if(inetReachable){
                        break;
                    }
                }
                if(inetReachable){
                    continue;
                }
            }

            // Mark state change and emit signal
            currentlyInetReachable = inetReachable;
            if(inetReachable){
                emit inetUp();
            } else {
                emit inetDown();
            }
        }
    }
    emit finished();
}
