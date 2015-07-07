#include "devicemodemutils.h"

QHash<QString, QString> DeviceModemUtils::getDeviceInfo(QSerialPortInfo *port, DeviceModemInfo *modem)
{
    // Hash with parsed modem infos
    QHash<QString, QString> atiParsed;

    // Keep reading device info until recognized or retries out
    for(int retry = 3; retry; retry--){

        // Send device specific info request
        QString atiAnswer = SerialDriver::sendAT(port, modem->get("AT_DEVICE_INFO"));

        // Parse the answer using pattern from modems.ini
        atiParsed = PatternParser::parse( modem->get("AT_DEVICE_INFO_ANSWER"),
                                          atiAnswer);

        // Check if manufacturer and model are present.
        // If so, assume correct answer and return.
        if(!atiParsed.value("manufacturer").isEmpty() &&
                !atiParsed.value("model").isEmpty()){
            // Modem found!
            break;
        }
    }
    return atiParsed;
}

bool DeviceModemUtils::initModem(QSerialPortInfo *port, DeviceModemInfo *modem)
{
    bool success = false;

    // Send modem init
    QString atAnswer = SerialDriver::sendAT(port, modem->get("AT_PREPARE"));
    if(atAnswer.indexOf("OK") != -1){
        success = true;
    }
    return success;
}

QString DeviceModemUtils::portSignature(QSerialPortInfo port)
{
    return port.portName()
            + port.vendorIdentifier()
            + port.productIdentifier()
            + port.description();
}
QString DeviceModemUtils::portSignaturePtr(QSerialPortInfo *port)
{
    return port->portName()
            + port->vendorIdentifier()
            + port->productIdentifier()
            + port->description();
}

bool DeviceModemUtils::hasPortSignature(DeviceModem *modem, QString portSig)
{
    bool has = false;
    if(modem->getUiPort() != NULL) {
        if(portSignaturePtr(modem->getUiPort()) == portSig) {
            has = true;
        }
    }
    if(modem->getModemPort() != NULL) {
        if(portSignaturePtr(modem->getModemPort()) == portSig) {
            has = true;
        }
    }
    return has;
}
