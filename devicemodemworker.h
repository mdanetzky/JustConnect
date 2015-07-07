#ifndef DEVICEMODEMWORKER_H
#define DEVICEMODEMWORKER_H

#include "devicemodem.h"
#include <QObject>
#include <QTimer>

class DeviceModemWorker: public QObject
{
    Q_OBJECT
public:
    DeviceModemWorker();
    ~DeviceModemWorker();

    DeviceModem *getModem() const;
    void setModem(DeviceModem *value);
    void stop();

public slots:
    void process();
    void sendAT(QString at);
signals:
    void message(QString, QString);
    void finished();
    void modemAnswer(QString);

private:
    DeviceModem *modem;
    QString imei;
    bool stopFlag;
};

#endif // DEVICEMODEMWORKER_H
