#ifndef RASDRIVER_H
#define RASDRIVER_H
#include <QLibrary>
#include <QList>
#include <QObject>
#include "rasdriverdefs.h"

class RasDriver: public QObject
{
    Q_OBJECT

public:
    RasDriver();
    static void init();
    static QStringList getPhonebookEntries();
    static QHash<QString, QString> getDevices();
    static QList<RASCONN> getActiveConnections();
    static QHash<QString, RASCONN> getConnections();
    static RasDriver *connect(QString com);
    static void createPhonebookEntry(QString name);
    static QHash<QString, QString> getTapiByCOM();
    static QString rasErrorString(UINT errCode);
private:
    static void WINAPI RasCallback(HRASCONN hrasconn,
                            UINT unMsg, RASCONNSTATE rascs,
                            DWORD dwError, DWORD dwExtendedError);
    static bool initialized;

    // RAS
    static QLibrary *rasDll;
    static _RasEnumEntries RasEnumEntries;
    static _RasGetErrorString RasGetErrorString;
    static _RasEnumConnections RasEnumConnections;
    static _RasGetConnectStatus RasGetConnectStatus;
    static _RasSetEntryProperties RasSetEntryProperties;
    static _RasEnumDevices RasEnumDevices;
    static _RasDial RasDial;

    // TAPI
    static QLibrary *tapiDll;
    static _lineInitializeEx lineInitializeEx;
    static _lineNegotiateAPIVersion lineNegotiateAPIVersion;
    static _lineOpen lineOpen;
    static _lineGetID lineGetID;
    static _lineGetDevCaps lineGetDevCaps;
    static _lineClose lineClose;

};

#endif // RASDRIVER_H
