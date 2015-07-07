#include "devicemodem.h"



void DeviceModem::setPort(QSerialPortInfo *port)
{
    if(port != NULL) {
        if(port->description() == getModemInfo()->get("UI_COM_DESCRIPTION")){
            uiPort = new QSerialPortInfo(*port);
        }
        if(port->description() == getModemInfo()->get("MODEM_COM_DESCRIPTION")){
            modemPort = new QSerialPortInfo(*port);
        }
    }
}

bool DeviceModem::allPortsSet()
{
    bool portsSet = false;
    if(modemInfo != NULL){

        // RAS connections need ui and modem port to be set
        if(modemInfo->get("AT_CONNECT_VIA") == "ras") {
            portsSet = (uiPort != NULL) && (modemPort != NULL);
        }

        // NDIS connections need only ui port
        // since network connection is establiched via network device
        if(modemInfo->get("AT_CONNECT_VIA") == "ndis") {
            portsSet = (uiPort != NULL);
        }
    }
    return portsSet;
}

QString DeviceModem::value(QString key)
{
    return data.value(key.toLower());
}

void DeviceModem::insert(QString key, QString value)
{
    data.insert(key.toLower(), value);
}
QHash<QString, QString> *DeviceModem::getMobileProfile()
{
    return mobileProfile;
}

void DeviceModem::setMobileProfile(QHash<QString, QString> *value)
{
    mobileProfile = value;
}
QSerialPortInfo *DeviceModem::getModemPort() const
{
    return modemPort;
}
QSerialPortInfo *DeviceModem::getUiPort() const
{
    return uiPort;
}
DeviceModemInfo *DeviceModem::getModemInfo() const
{
    return modemInfo;
}



