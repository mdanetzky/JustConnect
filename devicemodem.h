#ifndef DEVICEMODEM_H
#define DEVICEMODEM_H

#include "devicemodeminfo.h"
#include <QSerialPortInfo>
#include <QHash>
#include <QObject>

/*!
 * \brief Abstract DeviceModem class
 * Defines common modem functionalities.
 * \author Matthias Danetzky
 */
class DeviceModem: public QObject
{
    Q_OBJECT
public:
    DeviceModem(DeviceModemInfo *info){
        modemInfo = info;
        mobileProfile = NULL;
        modemPort = NULL;
        uiPort = NULL;
    }
    virtual ~DeviceModem(){}
    virtual void prepare() = 0;
    virtual bool checkSIM() = 0;
    virtual QString getIMSI() = 0;
    virtual int getNetStatus() = 0;
    virtual void connect() = 0;
    virtual void disconnect() = 0;

    void setPort(QSerialPortInfo *port);
    bool allPortsSet();

    QString value(QString key);
    void insert(QString key, QString value);

    QHash<QString, QString> *getMobileProfile();
    void setMobileProfile(QHash<QString, QString> *value);

    QSerialPortInfo *getModemPort() const;
    QSerialPortInfo *getUiPort() const;
    DeviceModemInfo *getModemInfo() const;

protected:
    QHash<QString, QString> data;
    DeviceModemInfo *modemInfo;
    QHash<QString, QString> *mobileProfile;
    QSerialPortInfo *modemPort;
    QSerialPortInfo *uiPort;
};

#endif // DEVICEMODEM_H
