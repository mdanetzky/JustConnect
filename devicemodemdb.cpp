#include "devicemodemdb.h"
#include "settingsdbutils.h"

void DeviceModemDB::init(){
    if(!initialized){

        // Prevent double init
        initialized = true;

        // TODO check if modems.ini exists

        QHash<QString, QHash<QString, QString> *> modemsRaw =
                SettingsDBUtils::parseSettings(":/Modems.ini");

        // Create DeviceModemInfo instances and populate main DB
        QHash<QString, QHash<QString, QString> *>::iterator modemsRawIterator;
        for (modemsRawIterator = modemsRaw.begin();
             modemsRawIterator != modemsRaw.end();
             modemsRawIterator++) {
            QHash<QString, QString> *entry = modemsRawIterator.value();
            DeviceModemInfo *info = new DeviceModemInfo(entry);
            modemDBbyName.insert(entry->value("name"), info);
        }

        // Debug output of populated DB
        //        QHash<QString, DeviceModemInfo *>::iterator modemDBbyNameIterator;
        //        for (modemsRawIterator = modemsRaw.begin();
        //             modemsRawIterator != modemsRaw.end();
        //             modemsRawIterator++) {

        //        }

        foreach (DeviceModemInfo *info, modemDBbyName.values()) {
            QList<QString> deviceKeys = info->getKeys();
            qSort(deviceKeys);
            qDebug();
            qDebug()<< "Device:" << info->getName();
            //            foreach (QString key, deviceKeys) {
            //                qDebug() << key.toUpper() << ": " << info->get(key).replace("\r", "\\r").replace("\n", "\\n");
            //            }
        }

        // Populate other DBs
        foreach (DeviceModemInfo *info, modemDBbyName.values()) {

            // PID VID db
            QString modemIdentifier = info->getVID()+"/"+info->getPID();
            if(info->get("parent").isEmpty()) {
                modemDBParentsByVid.insert(info->getVID(), info);
            }
            //qDebug() << "inserting pid/vid:" << modemIdentifier << " modem: " << info->getName();
            modemDBbyVidPid.insert(modemIdentifier, info);
            modemIdentifier = info->get("ati_manufacturer")+"/"+info->get("ati_model");
            if(modemIdentifier != "/") {
                modemDBbyAtiManufacturerModel.insert(modemIdentifier,info);
            }
            QString uiPort = info->get("UI_COM_DESCRIPTION");
            if(!uiPort.isEmpty()) {
                uiComPorts.append(uiPort);
            }
            QString modemPort = info->get("MODEM_COM_DESCRIPTION");
            if(!modemPort.isEmpty()) {
                modemComPorts.append(modemPort);
            }
        }

        uiComPorts.removeDuplicates();
        modemComPorts.removeDuplicates();
    }
}
DeviceModemInfo *DeviceModemDB::findMasterByVid(QString vid, QString pid){
    init();
    vid = DeviceModemInfo::formatVidPid(vid);
    pid = DeviceModemInfo::formatVidPid(pid);
    QString modemIdentifier = vid+"/"+pid;
    return modemDBbyVidPid.value(modemIdentifier);
}
DeviceModemInfo *DeviceModemDB::findMasterByVid(int vid, int pid){
    init();
    QString svid = DeviceModemInfo::formatVidPid(QString::number(vid, 16));
    QString spid = DeviceModemInfo::formatVidPid(QString::number(pid, 16));
    QString modemIdentifier = svid+"/"+spid;
    return modemDBbyVidPid.value(modemIdentifier);
}
DeviceModemInfo *DeviceModemDB::findMasterByVid(int vid){
    init();
    QString svid = DeviceModemInfo::formatVidPid(QString::number(vid, 16));
    QString modemIdentifier = svid+"/";
    return modemDBbyVidPid.value(modemIdentifier);
}

DeviceModemInfo *DeviceModemDB::findByName(QString name)
{
    return modemDBbyName.value(name);
}

DeviceModemInfo *DeviceModemDB::findByAtiManufacturerModel(QString manufacturer, QString model)
{
    QString modemIdentifier = manufacturer + "/" + model;
    return modemDBbyAtiManufacturerModel.value(modemIdentifier);
}
QStringList *DeviceModemDB::getUiComPorts()
{
    return &uiComPorts;
}

QStringList *DeviceModemDB::getModemComPorts()
{
    return &modemComPorts;
}

QHash<QString, DeviceModemInfo *> DeviceModemDB::modemDBParentsByVid;
QHash<QString, DeviceModemInfo *> DeviceModemDB::modemDBbyVidPid;
QHash<QString, DeviceModemInfo *> DeviceModemDB::modemDBbyName;
QHash<QString, DeviceModemInfo *> DeviceModemDB::modemDBbyAtiManufacturerModel;
bool DeviceModemDB::initialized = false;
QStringList DeviceModemDB::uiComPorts;
QStringList DeviceModemDB::modemComPorts;
