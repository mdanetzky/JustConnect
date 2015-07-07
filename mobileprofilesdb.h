#ifndef MOBILEPROFILESDB_H
#define MOBILEPROFILESDB_H

#include <QHash>
#include <QString>

class MobileProfilesDB
{
public:
    static void init();
    static QHash<QString, QString> *findByIMSI(QString imsi);
private:
    static QHash<QString, QHash<QString, QString> *> dbByProfileName;
    static QHash<QString, QHash<QString, QString> *> dbByMccMnc;
    static bool initialized;
};

#endif // MOBILEPROFILESDB_H
