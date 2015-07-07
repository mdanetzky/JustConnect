#include "applicationskiller.h"
#include "applicationskillerworker.h"
#include <QThread>

ApplicationsKillerWorker *ApplicationsKiller::startKillerThread()
{
    QThread* thread = new QThread;
    ApplicationsKillerWorker* worker = new ApplicationsKillerWorker();
    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    return worker;
}
