#include "devicemodemworker.h"
#include "serialdriver.h"
#include "mobileprofilesdb.h"
#include <QThread>

DeviceModemWorker::DeviceModemWorker()
{
    stopFlag = false;
}

DeviceModemWorker::~DeviceModemWorker()
{
    disconnect();
}
DeviceModem *DeviceModemWorker::getModem() const
{
    return modem;
}

void DeviceModemWorker::setModem(DeviceModem *value)
{
    modem = value;
    if(modem != NULL){
        imei = modem->value("imei");
    }
}

void DeviceModemWorker::stop()
{
    qDebug() << "stopping modem worker";
    stopFlag = true;
}

void DeviceModemWorker::process()
{
    if(modem != NULL){
        if(stopFlag){
            emit finished();
            return;
        }
        if(modem->value("simready").isEmpty()){
            if(modem->checkSIM()){
                modem->insert("simready", "ready");
            } else {
                emit message(imei, "missing SIM!");
                // Nothing more to do
                // Quit thread
                emit finished();
                return;
            }
        }
        if(modem->getMobileProfile() == NULL){
            QString imsi = modem->getIMSI();
            qDebug() << imsi;
            QHash<QString, QString> *mobProfile = MobileProfilesDB::findByIMSI(imsi);
            if(mobProfile == NULL){
                emit message(imei, "Mobile Profile not found!");
                // Nothing more to do
                // Quit thread
                emit finished();
                return;
            }
            modem->setMobileProfile(mobProfile);
        }
        if(modem->value("netready").isEmpty()){
            // TODO wait for network ready or die
            int netStat = modem->getNetStatus();
            if(netStat == 1 ){
                modem->insert("netready","connected");
                emit message(imei, "Network found");
            } else if(netStat == 5 ){
                modem->insert("netready","roaming");
                emit message(imei, "roaming");
            } else {
                QTimer::singleShot(1000, this, SLOT(process()));
                return;
            }
            qDebug() << modem->value("imei") << "NET FOUND!";
        }
        if(modem->value("netready") == "connected" ||
                modem->value("netready") == "roaming"){
            if(modem->value("internet").isEmpty()){
                modem->connect();
                modem->insert("internet", "up");
            }
            QTimer::singleShot(1000, this, SLOT(process()));
            return;
        }
    }
    qDebug() << "exitin modem thread";
    emit finished();
}

void DeviceModemWorker::sendAT(QString at)
{
    qDebug() << "send command received" << at;
    QString answer = SerialDriver::sendAT(modem->getUiPort(), at);
    emit modemAnswer(answer);
}

