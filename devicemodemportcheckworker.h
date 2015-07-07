#ifndef DEVICEMODEMPORTCHECKWORKER_H
#define DEVICEMODEMPORTCHECKWORKER_H
#include <QObject>
#include <QSerialPortInfo>
#include <QHash>
#include <QDebug>
#include "devicemodemdb.h"
#include "devicemodeminfo.h"
#include "serialdriver.h"
#include "patternparser.h"
#include "devicemodemutils.h"

/*!
 * Worker, that checks a single com port to determine the device's imei.
 *
 * \author Matthias Danetzky
 */
class DeviceModemPortCheckWorker: public QObject
{
    Q_OBJECT
public:
    DeviceModemPortCheckWorker();
    void setPort(QSerialPortInfo port);
public slots:
    void process();

signals:
    void imeiFound(QString, DeviceInfoHash);
    void deviceNotReady(QString);
    void finished();
    void deviceNotRecognized(QString portName);

private:
    QSerialPortInfo portInfo;
};

#endif // DEVICEMODEMPORTCHECKWORKER_H
