#include "devicemodemportcheckworker.h"

DeviceModemPortCheckWorker::DeviceModemPortCheckWorker(){}

void DeviceModemPortCheckWorker::setPort(QSerialPortInfo port)
{
    portInfo = port;
}

void DeviceModemPortCheckWorker::process()
{
    // Check if port has a vendor identifier
    if(portInfo.hasVendorIdentifier()){
        QHash<QString, QString> infoParsed;
        qDebug() << "VID:" << QString::number(portInfo.vendorIdentifier(), 16)
                 << " PID:" << QString::number(portInfo.productIdentifier(), 16)
                 << " COM:" << portInfo.portName()
                 << " COM NAME:" << portInfo.systemLocation()
                 << " DESC:" << portInfo.description();

        // Find generic modem description by Vendor id
        DeviceModemInfo *modemInfo = DeviceModemDB::findMasterByVid(portInfo.vendorIdentifier());
        if(modemInfo == NULL) {
            // Vendor ID missing in modems.ini
            emit deviceNotRecognized(portInfo.portName());
        } else {
//            if(true) {
            if(DeviceModemUtils::initModem(&portInfo, modemInfo)) {
                infoParsed = DeviceModemUtils::getDeviceInfo(&portInfo, modemInfo);
                QString imei = infoParsed.value("imei");
                if(imei.isEmpty()){
                    // Could not determine imei
                    // Let's assume, that the device is not ready
                    emit deviceNotReady(portInfo.portName());
                } else {
                    // imei found
                    // Add port name and description and emit in signal
                    infoParsed.insert("port", portInfo.portName());
                    infoParsed.insert("portDesc", portInfo.description());
                    emit imeiFound(portInfo.portName(), infoParsed);
                }
            } else {
                // Device couldn't be initialized
                emit deviceNotReady(portInfo.portName());
            }
        }
    } else {
        // Port has no vendor identifier
        // Probably still uninitialized
        emit deviceNotReady(portInfo.portName());
    }

    // Finish this thread
    emit finished();
}
