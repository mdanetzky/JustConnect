#include "devicemodeminfo.h"

DeviceModemInfo::DeviceModemInfo()
{
//    ModemStatus stat;
//    status = stat;
}
DeviceModemInfo::DeviceModemInfo(QHash<QString, QString> *data)
{
//    ModemStatus stat;
//    status = stat;
//    status = ModemStatus();
    rawData = data;
    name = data->value("name");
    vid = formatVidPid(data->value("vid"));
    pid = formatVidPid(data->value("pid"));
    comDescription = data->value("ui_com_description");
}
QString DeviceModemInfo::formatVidPid(QString id){
    if(!id.isEmpty()){
        id = id.toLower();
        if(!id.startsWith("0x")){
            id = "0x" + id;
        }
    }
    return id;
}
QString DeviceModemInfo::get(QString key){
    return rawData->value(key.toLower());
}

void DeviceModemInfo::set(QString key, QString value)
{
    rawData->insert(key, value);
}
QString DeviceModemInfo::getSerialPort() const
{
    return serialPort;
}

void DeviceModemInfo::setSerialPort(const QString &value)
{
    serialPort = value;
}
//QSerialPortInfo &DeviceModemInfo::getSerialPortInfo()
//{
//    return serialPortInfo;
//}

//void DeviceModemInfo::setSerialPortInfo(const QSerialPortInfo &value)
//{
//    serialPortInfo = value;
//}
ModemStatus &DeviceModemInfo::getStatus()
{
    return status;
}

void DeviceModemInfo::setStatus(QHash<QString, QString> data)
{
    status.set(data);
}

void DeviceModemInfo::fillDefaults(DeviceModemInfo *defaultDevice)
{
    foreach (QString defKey, defaultDevice->getKeys()) {
        if(!rawData->contains(defKey)){
            rawData->insert(defKey, defaultDevice->get(defKey));
        }
    }
    if(pid.isEmpty()){
        pid = defaultDevice->getPID();
    }
    if(vid.isEmpty()){
        vid = defaultDevice->getVID();
    }
    if(comDescription.isEmpty()){
        comDescription = defaultDevice->getComDescription();
    }
}

QList<QString> DeviceModemInfo::getKeys()
{
    return rawData->keys();
}

//QHash<QString, QString> DeviceModemInfo::getData()
//{
//    return rawData;
//}
//DeviceModem *DeviceModemInfo::getDeviceDriver() const
//{
//    return deviceDriver;
//}

//void DeviceModemInfo::setDeviceDriver(DeviceModem *value)
//{
//    deviceDriver = value;
//}




QString DeviceModemInfo::getName(){
    return name;
}
QString DeviceModemInfo::getVID(){
    return vid;
}
QString DeviceModemInfo::getPID(){
    return pid;
}
QString DeviceModemInfo::getComDescription(){
    return comDescription;
}
