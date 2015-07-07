#ifndef DEVICEMODEMSEARCHINGWORKER_H
#define DEVICEMODEMSEARCHINGWORKER_H
#include <QObject>
#include <QString>
#include <QThread>
#include <QList>
#include <QHash>
#include <QSerialPortInfo>
#include "devicemodem.h"
#include "devicemodemdb.h"
#include "devicemodemportcheckworker.h"
#include "devicemodemutils.h"
#include "devicemodemhuawei.h"

/*!
 * Worker, that constantly loops through available com ports
 * and seeks new modems.
 * If new modem is found - its instance is emitted via deviceFound signal
 * If a modem disappears - its imei and name are emitted via deviceRemoved
 */
class DeviceModemSearchingWorker: public QObject
{
    Q_OBJECT

public:
    DeviceModemSearchingWorker();

public slots:
    void process();
    void deviceNotReady(QString comName);
    void deviceNotRecognized(QString comName);
    void imeiFound(QString comName, DeviceInfoHash deviceInfo);

signals:
    void deviceFound(DeviceModem *);

    /*!
     * \brief Signals removed device by sending its imei and name
     */
    void deviceRemoved(QString imei, QString name);
    void finished();

private:
    void startPortCheckWorker(QSerialPortInfo portInfo);
    QList<QString> portsRecognized;
    QList<QString> portsBeingChecked;
    QHash<QString, QSerialPortInfo> portInfoByName;
    QHash<QString, DeviceModem *> devicesByImei;
    QHash<QString, QHash<QString, QString> > deviceInfoByImei;
};

#endif // DEVICEMODEMSEARCHINGWORKER_H
