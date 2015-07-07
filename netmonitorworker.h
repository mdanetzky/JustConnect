#ifndef NETMONITORWORKER_H
#define NETMONITORWORKER_H

#include <QObject>

class NetMonitorWorker: public QObject
{
    Q_OBJECT

public:
    NetMonitorWorker();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);
    void inetDown();
    void inetUp();
};

#endif // NETMONITORWORKER_H
