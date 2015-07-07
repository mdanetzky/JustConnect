#include "justconnect.h"
#include "netmonitor.h"
#include "devicemodem.h"
#include "devicemodemworker.h"
#include "devicemodemsearchingworker.h"
#include "applicationskiller.h"
#include "applicationskillerworker.h"
#include "mobileprofilesdb.h"
#include "rasdriver.h"
#include <QDebug>

JustConnect::JustConnect(){}

JustConnect::~JustConnect()
{
    UITrayIcon::remove();
}

void JustConnect::init()
{
    uiDiags.show();

    RasDriver::init();
    // Read Modems.ini and populate DeviceModem databases
    DeviceModemDB::init();

    // Read MobileProfiles.ini
    MobileProfilesDB::init();

    // Prepare system tray icon
    UITrayIcon::setIcon(":/antenna_with_bars.svg");
    UITrayIcon::connectIcon(this, SLOT(trayIconClick(QSystemTrayIcon::ActivationReason)));

    // Start internet connection monitoring
    netMonitorWorker = NetMonitor::startNetMonitoring();
    connect(netMonitorWorker, SIGNAL(inetUp()), this, SLOT(inetUp()));
    connect(netMonitorWorker, SIGNAL(inetDown()), this, SLOT(inetDown()));

    // Start mobile (unwanted) applications clobber
    ApplicationsKillerWorker *akWorker = ApplicationsKiller::startKillerThread();
    connect(akWorker, SIGNAL(processKilled(QString)), this, SLOT(processKilled(QString)));

    // Start searching for new modems
    DeviceModemSearchingWorker *modemSearchWorker = new DeviceModemSearchingWorker();
    QThread *modemSearchThread = new QThread;
    modemSearchWorker->moveToThread(modemSearchThread);

    // Connect thread signals
    connect(modemSearchThread, SIGNAL(started()), modemSearchWorker, SLOT(process()));
    connect(modemSearchWorker, SIGNAL(finished()), modemSearchThread, SLOT(quit()));
    connect(modemSearchWorker, SIGNAL(finished()), modemSearchWorker, SLOT(deleteLater()));
    connect(modemSearchThread, SIGNAL(finished()), modemSearchThread, SLOT(deleteLater()));

    // Connect payload related signals
    connect(modemSearchWorker, SIGNAL(deviceFound(DeviceModem *)),
            this, SLOT(deviceFound(DeviceModem *)));
    connect(modemSearchWorker, SIGNAL(deviceRemoved(QString,QString)),
            this, SLOT(modemRemoved(QString,QString)));

    // start the thread
    modemSearchThread->start();

//    modemMonitorWorker = DeviceModemFactory::startSeekingDevices();
//    connect(&DeviceModemFactory::instance, SIGNAL(deviceFound(DeviceModem*)),
//            this, SLOT(deviceFound(DeviceModem*)));
//    connect(&DeviceModemFactory::instance, SIGNAL(modemRemoved(QString,QString)),
//            this, SLOT(modemRemoved(QString,QString)));
}

void JustConnect::inetUp()
{
    UITrayIcon::showMessage("Internet connection established");
}
void JustConnect::inetDown()
{
    UITrayIcon::showMessage("Internet connection lost");
}
void JustConnect::deviceFound(DeviceModem *modem)
{
    if(modem != NULL){
        QString imei = modem->value("imei");
        if(!imei.isEmpty()){
            QThread* thread = new QThread;
            DeviceModemWorker* modemWorker = new DeviceModemWorker();
            modemWorker->moveToThread(thread);
            modemWorker->setModem(modem);
            connect(thread, SIGNAL(started()), modemWorker, SLOT(process()));
            connect(modemWorker, SIGNAL(finished()), thread, SLOT(quit()));
            connect(modemWorker, SIGNAL(finished()), modemWorker, SLOT(deleteLater()));
            connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
            thread->start();
            modemWorkersByImei.insert(imei, modemWorker);
            connect(modemWorker, SIGNAL(message(QString,QString)),
                    this, SLOT(modemMessage(QString,QString)));
            UITrayIcon::showMessage(modemWorker->getModem()->getModemInfo()->getName() + " found");

            // Diagnostics
            connect(&uiDiags, SIGNAL(commandAT(QString)), modemWorker, SLOT(sendAT(QString)), Qt::QueuedConnection);
            connect(modemWorker, SIGNAL(modemAnswer(QString)), &uiDiags, SLOT(modemAnswer(QString)), Qt::QueuedConnection);
        }
    }

}

void JustConnect::modemRemoved(QString imei, QString name)
{
    qDebug() << "modem remove arrived!";
    if(imei != NULL){
        qDebug() << "modem remove imei" << imei;
        if(!imei.isEmpty()){
            UITrayIcon::showMessage(name + " disconnected");
            DeviceModemWorker *mWorker = modemWorkersByImei.value(imei);
            if(mWorker != NULL){
                mWorker->stop();
            }
        }
    }
}

void JustConnect::trayIconClick(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        qDebug() << "Icon click";
        //        if(this->isVisible()){
        //            this->hide();
        //        } else {
        //            this->show();
        //        }
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "Icon dblclick";
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "Icon middle click";
        break;
    case QSystemTrayIcon::Context:
        qDebug() << "Icon right click";
        break;
    default:
        ;
    }
}

void JustConnect::processKilled(QString name)
{
    UITrayIcon::showMessage(name + " was disabled");
}

void JustConnect::modemMessage(QString sourceImei, QString message)
{
    DeviceModemWorker *mWorker = modemWorkersByImei.value(sourceImei);
    UITrayIcon::showMessage(mWorker->getModem()->getModemInfo()->getName() + " " + message);
}



