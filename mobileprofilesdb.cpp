#include "mobileprofilesdb.h"
#include "settingsdbutils.h"
#include <QDebug>

void MobileProfilesDB::init()
{
    if(!initialized){
        initialized = true;

        dbByProfileName = SettingsDBUtils::parseSettings(":/MobileProfiles.ini");

        // Populate other dbs
        QHash<QString, QHash<QString, QString> *>::iterator dbProfileIterator;
        for(dbProfileIterator = dbByProfileName.begin();
            dbProfileIterator != dbByProfileName.end();
            dbProfileIterator++){

            dbByMccMnc.insert(dbProfileIterator.value()->value("mcc") +
                              dbProfileIterator.value()->value("mnc"),
                              dbProfileIterator.value());
        }
        //qDebug() << dbByProfileName;
    }
}

QHash<QString, QString> *MobileProfilesDB::findByIMSI(QString imsi)
{
    init();
    QHash<QString, QString> *retVal = NULL;
    if(imsi.length() > 6){
        QString mccMnc5 = imsi.mid(0,5);
        QString mccMnc6 = imsi.mid(0,6);
        retVal = dbByMccMnc.value(mccMnc5);
        if(retVal == NULL){
            retVal = dbByMccMnc.value(mccMnc6);
        }
    }
    return retVal;
}

QHash<QString, QHash<QString, QString> *> MobileProfilesDB::dbByProfileName;
QHash<QString, QHash<QString, QString> *> MobileProfilesDB::dbByMccMnc;
bool MobileProfilesDB::initialized = false;
