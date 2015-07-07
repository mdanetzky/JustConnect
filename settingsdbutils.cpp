#include "settingsdbutils.h"

#include <QSettings>
#include <QDebug>

QHash<QString, QHash<QString, QString> *> SettingsDBUtils::parseSettings(QString iniFileName)
{
    QHash<QString, QHash<QString, QString> *> db;
    QHash<QString, QString> *defaultRecord;
    QSettings ini(iniFileName, QSettings::IniFormat);

    // Read Hash tables from ini file
    // Format: Hash (GroupName -> Hash (EntryName -> StringValue))
    foreach (QString group, ini.childGroups()) {
        ini.beginGroup(group);
        QHash<QString, QString> *entryData = new QHash<QString, QString>();
        foreach (QString key, ini.childKeys()) {
            QString lowerKey = key.toLower().trimmed();
            entryData->insert(lowerKey, ini.value(key).toString());
        }
        ini.endGroup();
        entryData->insert("name", group);
        db.insert(group, entryData);
        if(group == "Default") {
            defaultRecord = entryData;
        }
    }

    // Set defaults from parents (Only one level supported!)
    QHash<QString, QHash<QString, QString> *>::iterator record;
    for (record = db.begin(); record != db.end(); record++) {
        QString parentName = record.value()->value("parent", "");
        if(!parentName.isEmpty()){
            QHash<QString, QString> *parentRecord = db.value(parentName);
            QHash<QString, QString>::iterator parentEntry;
            for (parentEntry = parentRecord->begin();
                 parentEntry != parentRecord->end(); parentEntry++) {
                if(!record.value()->contains(parentEntry.key())){
                    record.value()->insert(parentEntry.key(), parentEntry.value());
                }
            }
        }
    }

    // Set defaults from the "Default" record
    for (record = db.begin(); record != db.end(); record++) {
        QHash<QString, QString>::iterator entry;
        for (entry = defaultRecord->begin();
             entry != defaultRecord->end(); entry++) {
            if(!record.value()->contains(entry.key())){
                record.value()->insert(entry.key(), entry.value());
            }
        }
    }

    return db;
}
