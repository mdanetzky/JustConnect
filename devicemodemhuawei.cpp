#include "devicemodemhuawei.h"
#include "serialdriver.h"
#include "patternparser.h"
#include "nicdriver.h"
#include "rasdriver.h"
#include <QHash>
#include <QThread>

DeviceModemHuawei::DeviceModemHuawei(DeviceModemInfo *info)
    :DeviceModem(info)
{
}

DeviceModemHuawei::~DeviceModemHuawei()
{
    // Delete private copy of serialPortInfo
}

void DeviceModemHuawei::getStatus(){
    if(modemInfo->get("ATI_STATUS").isEmpty()){
        QString ati = SerialDriver::sendAT(getUiPort(), "AT_DEVICE_INFO");
        modemInfo->set("ATI_STATUS", ati);
    }
    QString atStatus = modemInfo->get("AT_STATUS_QUERY");
    QString sInfo = SerialDriver::sendAT(getUiPort(), atStatus);
    qDebug() << "Sysinfo: " << sInfo;
    QHash<QString, QString> parsedInfo = PatternParser::parse(modemInfo->get("AT_STATUS_ANSWER"),sInfo);
    qDebug() << "parsed status : " << parsedInfo;
    modemInfo->setStatus(parsedInfo);
}

void DeviceModemHuawei::connect()
{
    // Determine APN
    QString apn = getMobileProfile()->value("apn");
    if(!apn.isEmpty()){

        // Check if modem should be connected via ndis or ras
        // depending on modems.ini entry
        QString connectVia = modemInfo->get("AT_CONNECT_VIA").toLower();
        QString connectString = "";
        if(connectVia == "ndis"){
            connectString = modemInfo->get("AT_CONNECT_NDIS");
            connectString = PatternParser::fill(connectString, getMobileProfile());
            SerialDriver::sendAT(getUiPort(), connectString);
            NicDriver *nic = NULL;
            int repeats = 3;
            while(repeats-- && nic == NULL){
                nic = NicDriver::getDriverByIMEI(DeviceModem::value("imei"));
                if(nic == NULL) {
                    QThread::msleep(1000);
                }
            }
            if(nic == NULL){
                // Fallback to RAS
                connectVia == "ras";
            } else {
                nic->connect();
            }
        }
        if(connectVia == "ras"){
            RasDriver::init();
            connectString = modemInfo->get("AT_CONNECT_RAS");
            connectString = PatternParser::fill(connectString, getMobileProfile());
            SerialDriver::sendAT(getUiPort(), connectString);
            RasDriver::connect(getModemPort()->portName());
        }
    }
}

void DeviceModemHuawei::disconnect()
{

}

bool DeviceModemHuawei::checkSIM()
{
    bool hasSim = false;
    int retries = 5;
    forever{
        QString simStatus = SerialDriver::sendAT(getUiPort(), modemInfo->get("AT_CHECK_SIM"));
        if(simStatus == "+CME ERROR: 13"){
            // Unknown SIM state retry
            QThread::msleep(500);
            if(retries--){
                continue;
            }
        }
        if(simStatus == "+CME ERROR: 14"){
            // SIM busy - give it some time
            QThread::msleep(300);
            continue;
        }
        if(simStatus == "+CPIN: READY\rOK"){
            hasSim = true;
        }
        break;
    }
    return hasSim;
}

QString DeviceModemHuawei::getIMSI()
{
    QString imsi = SerialDriver::sendAT(getUiPort(), modemInfo->get("AT_IMSI"));
    if(imsi.endsWith("\rOK")){
        imsi = imsi.mid(0, imsi.length() - 3);
    }
    return imsi;
}


void DeviceModemHuawei::prepare()
{
    SerialDriver::sendAT(getUiPort(), modemInfo->get("AT_PREPARE"));
}

int DeviceModemHuawei::getNetStatus()
{
    int retVal = -1;
    QString netStatus = SerialDriver::sendAT(getUiPort(), modemInfo->get("AT_NET_STATUS"));
    QHash<QString, QString> parsedStatus = PatternParser::parse(modemInfo->get("AT_NET_STATUS_ANSWER"),netStatus);
    if(parsedStatus.contains("stat")){
        bool conversionOK;
        retVal = parsedStatus.value("stat").toInt(&conversionOK);
        if(!conversionOK){
            retVal = -1;
        }
    }
    return retVal;
}
