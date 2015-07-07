#include "devicemodemsearchingworker.h"
#include <QTimer>

DeviceModemSearchingWorker::DeviceModemSearchingWorker(){}

void DeviceModemSearchingWorker::process()
{
    // Get all available com ports
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QString> currentPorts;

    // Iterate through all system ports
    foreach (QSerialPortInfo portInfo, ports) {
        QString portSig = DeviceModemUtils::portSignature(portInfo);
        currentPorts.append(portSig);

        portInfoByName.insert(portInfo.portName(), portInfo);

        // Check if there is a new port in system
        if(!portsRecognized.contains(portSig) &&
                !portsBeingChecked.contains(portSig)) {

            // Check if port description is listed in modems db
            if(DeviceModemDB::getUiComPorts()->contains(portInfo.description()) ||
                    DeviceModemDB::getModemComPorts()->contains(portInfo.description())) {

                // Start port check worker
                portsBeingChecked.append(portSig);
                startPortCheckWorker(portInfo);
            } else {

                // Port does not belong to any device from modems.ini
                // Mark it as recognized to prevent further checks.
                portsRecognized.append(portSig);
            }
        }
    }

    // Check if any port has disappeared from the system
    foreach(QString portSig, portsRecognized) {
        if(!currentPorts.contains(portSig)) {

            qDebug() << portSig << "was removed";
            // Port removed from the system!
            portsRecognized.removeAll(portSig);
            portsBeingChecked.removeAll(portSig);

            // Find corresponding modem
            foreach (DeviceModem *modem, devicesByImei.values()) {
                if(DeviceModemUtils::hasPortSignature(modem, portSig)) {

                    // Emit device removed
                    emit deviceRemoved(modem->value("imei"), modem->getModemInfo()->getName());
                    devicesByImei.remove(modem->value("imei"));
                }
            }
        }
    }

    // Repeat
    QTimer::singleShot(1000, this, SLOT(process()));
}

void DeviceModemSearchingWorker::deviceNotReady(QString comName)
{
    qDebug() << comName << "Device not ready";
    portsBeingChecked.removeAll(DeviceModemUtils::portSignature(portInfoByName.value(comName)));
}

void DeviceModemSearchingWorker::deviceNotRecognized(QString comName)
{
    portsBeingChecked.removeAll(DeviceModemUtils::portSignature(portInfoByName.value(comName)));
    portsRecognized.append(DeviceModemUtils::portSignature(portInfoByName.value(comName)));

    // TODO inform about unrecognized device
}

void DeviceModemSearchingWorker::imeiFound(QString comName, DeviceInfoHash deviceInfo)
{
    QString imei = deviceInfo.value("imei");
    qDebug() << comName << "imei found" << imei;
    if(imei.isEmpty()) {

        // Somehow the device hasn't been recognized properly
        portsBeingChecked.removeAll(DeviceModemUtils::portSignature(portInfoByName.value(comName)));
    } else {

        // Check if modem driver is already instantiated
        DeviceModem *modemDriver = devicesByImei.value(imei, NULL);
        if(modemDriver == NULL) {

            // Modem driver not instantiated yet
            // Retrieve modem info
            DeviceModemInfo *modemInfo = DeviceModemDB::findByAtiManufacturerModel(deviceInfo.value("manufacturer"),
                                                                                   deviceInfo.value("model"));
            if(modemInfo == NULL) {

                // This modem's definition wasn't found in modems.ini
                // TODO maybe try to use the generic driver

            } else {

                // Create an instance of modem driver
                QString className = modemInfo->get("class");
                if(className == "DeviceModemHuawei"){
                    modemDriver = new DeviceModemHuawei(modemInfo);
                }
                modemDriver->insert("imei", imei);
            }
        }

        if(modemDriver == NULL) {

            // Modem driver couldn't be instantiated
            // TODO dump and/or send info about unknown modem

        } else {

            // Set serial port
            QSerialPortInfo portInfo = portInfoByName.value(comName);
            modemDriver->setPort(&portInfo);

            // Store driver locally
            devicesByImei.insert(imei, modemDriver);

            // If all set emit driver
            if(modemDriver->allPortsSet()) {
                emit deviceFound(modemDriver);
            }
        }

        // Mark port as recognized
        portsBeingChecked.removeAll(DeviceModemUtils::portSignature(portInfoByName.value(comName)));
        portsRecognized.append(DeviceModemUtils::portSignature(portInfoByName.value(comName)));
    }
}

void DeviceModemSearchingWorker::startPortCheckWorker(QSerialPortInfo portInfo)
{
    // Prepare the thread
    qDebug() << "Start checking thread for port" << portInfo.portName();
    QThread* thread = new QThread;
    DeviceModemPortCheckWorker* worker = new DeviceModemPortCheckWorker();
    worker->moveToThread(thread);
    worker->setPort(portInfo);

    // Connect thread signals
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    qRegisterMetaType<DeviceInfoHash>("DeviceInfoHash");

    // Connect payload related signals
    connect(worker, SIGNAL(deviceNotReady(QString)),
            this, SLOT(deviceNotReady(QString)));
    connect(worker, SIGNAL(deviceNotRecognized(QString)),
            this, SLOT(deviceNotRecognized(QString)));
    connect(worker, SIGNAL(imeiFound(QString, DeviceInfoHash)),
            this, SLOT(imeiFound(QString, DeviceInfoHash)));

    // start the thread
    thread->start();
}
