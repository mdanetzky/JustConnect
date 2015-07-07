#ifndef DEVICEMODEMINFO_H
#define DEVICEMODEMINFO_H

#include <QHash>
#include <QString>
#include <QtSerialPort/QSerialPortInfo>
#include "modemstatus.h"

class DeviceModemInfo
{
public:
    DeviceModemInfo();
    DeviceModemInfo(QHash<QString, QString> *data);
    QString getName();
    QString getVID();
    QString getPID();
    QString getComDescription();
    static QString formatVidPid(QString id);
    QString get(QString key);
    void set(QString key, QString value);
    QString getSerialPort() const;
    void setSerialPort(const QString &value);
 //   QSerialPortInfo &getSerialPortInfo();
 //   void setSerialPortInfo(const QSerialPortInfo &value);
    ModemStatus &getStatus();
    void setStatus(QHash<QString, QString> data);
    void fillDefaults(DeviceModemInfo *defaultDevice);
    QList<QString> getKeys();
//    QHash<QString, QString> getData();

private:
    QHash<QString, QString> *rawData;
    QString name;
    QString vid;
    QString pid;
    QString comDescription;
    QString serialPort;
    QSerialPortInfo serialPortInfo;
    ModemStatus status;
};


#endif // DEVICEMODEMINFO_H
