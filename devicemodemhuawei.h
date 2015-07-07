#ifndef DEVICEMODEMHUAWEI_H
#define DEVICEMODEMHUAWEI_H

#include "devicemodem.h"
#include <QObject>

class DeviceModemHuawei: public DeviceModem
{

public:
    DeviceModemHuawei(DeviceModemInfo *info);
    ~DeviceModemHuawei();
    void getStatus();

private:

    // DeviceModem interface
public:
    void connect();
    void disconnect();
    bool checkSIM();
    QString getIMSI();
    void prepare();
    QString value(QString key);
    int getNetStatus();
};

#endif // DEVICEMODEMHUAWEI_H
