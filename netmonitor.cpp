#include "netmonitor.h"
#include <QThread>

NetMonitor::NetMonitor()
{
}

bool NetMonitor::testTcpReachable(QString address, int port)
{
    bool retVal = false;
    QTcpSocket *testSocket = new QTcpSocket();
    testSocket->connectToHost(address, port);
    if(testSocket->waitForConnected(2000)){
        retVal = true;
    }
    testSocket->close();
    delete testSocket;
    return retVal;

}

NetMonitorWorker *NetMonitor::startNetMonitoring()
{
    QThread* thread = new QThread;
    NetMonitorWorker* worker = new NetMonitorWorker();
    worker->moveToThread(thread);
    //connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    return worker;
}
