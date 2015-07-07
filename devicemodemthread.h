#ifndef DEVICEMODEMTHREAD_H
#define DEVICEMODEMTHREAD_H

#include <QThread>

class DeviceModemThread: public QThread
{
    Q_OBJECT

public:
    DeviceModemThread();
};

#endif // DEVICEMODEMTHREAD_H
