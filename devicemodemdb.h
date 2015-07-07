#ifndef DEVICEMODEMDB_H
#define DEVICEMODEMDB_H
#include "devicemodeminfo.h"
//#include "settingsdbutils.h"
#include <QString>
#include <QSettings>
#include <QFile>
#include <QtCore/QDebug>

//class QString;
//class QHash<class T, class U>;

class DeviceModemDB
{
public:
    static void init();
    static DeviceModemInfo *findMasterByVid(QString vid, QString pid);
    static DeviceModemInfo *findMasterByVid(int vid, int pid);
    static DeviceModemInfo *findMasterByVid(int vid);
    static DeviceModemInfo *findByName(QString name);
    static DeviceModemInfo *findByAtiManufacturerModel(QString manufacturer, QString model);
    static QStringList *getUiComPorts();
    static QStringList *getModemComPorts();

private:
    static QHash<QString, DeviceModemInfo *> modemDBbyVidPid;
    static QHash<QString, DeviceModemInfo *> modemDBParentsByVid;
    static QHash<QString, DeviceModemInfo *> modemDBbyName;
    static QHash<QString, DeviceModemInfo *> modemDBbyAtiManufacturerModel;
    static QStringList uiComPorts;
    static QStringList modemComPorts;
    static bool initialized;
};

#endif // DEVICEMODEMDB_H
