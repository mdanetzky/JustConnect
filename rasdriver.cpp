#include "rasdriver.h"
#include <QDebug>

RasDriver::RasDriver(){}

void RasDriver::init()
{
    if(!initialized){

        // Prevent multiple inits
        initialized = true;

        // Load RAS dll
        rasDll = new QLibrary("rasapi32.dll");
        rasDll->load();
        if(rasDll->isLoaded()){
            RasEnumEntries = (_RasEnumEntries)rasDll->resolve("RasEnumEntriesW");
            RasGetErrorString = (_RasGetErrorString)rasDll->resolve("RasGetErrorStringW");
            RasEnumConnections = (_RasEnumConnections)rasDll->resolve("RasEnumConnectionsW");
            RasGetConnectStatus = (_RasGetConnectStatus)rasDll->resolve("RasGetConnectStatusW");
            RasSetEntryProperties = (_RasSetEntryProperties)rasDll->resolve("RasSetEntryPropertiesW");
            RasEnumDevices = (_RasEnumDevices)rasDll->resolve("RasEnumDevicesW");
            RasDial = (_RasDial)rasDll->resolve("RasDialW");

        }

        // Load TAPI 2 dll
        tapiDll = new QLibrary("tapi32.dll");
        tapiDll->load();
        if(tapiDll->isLoaded()){
            lineInitializeEx = (_lineInitializeEx)tapiDll->resolve("lineInitializeExW");
            lineNegotiateAPIVersion = (_lineNegotiateAPIVersion)tapiDll->resolve("lineNegotiateAPIVersion");
            lineOpen = (_lineOpen)tapiDll->resolve("lineOpenW");
            lineGetID = (_lineGetID)tapiDll->resolve("lineGetIDW");
            lineGetDevCaps = (_lineGetDevCaps)tapiDll->resolve("lineGetDevCapsW");
            lineClose = (_lineClose)tapiDll->resolve("lineClose");

        }

        // Test
        qDebug() << getPhonebookEntries();
        //qDebug() << getActiveConnections();
        qDebug() << getDevices();
        qDebug() << getTapiByCOM();
        //connect("COM46");
    }
}

QStringList RasDriver::getPhonebookEntries()
{
    QStringList retVal;
    RASENTRYNAME* entries = new RASENTRYNAME[1];
    entries[0].dwSize = sizeof(RASENTRYNAME);
    DWORD numEntries;
    DWORD size = entries[0].dwSize;
    DWORD res = RasEnumEntries(0, 0, entries, &size, &numEntries);

    // More than one entry
    if (res == ERROR_BUFFER_TOO_SMALL) {
        // Allocate enough memory to get
        // all the phonebook entries.
        delete[] entries;
        entries = new RASENTRYNAME[numEntries];
        entries[0].dwSize = sizeof(RASENTRYNAME);
        res = RasEnumEntries(0, 0, entries, &size, &numEntries);
        if (res) {
            qDebug() << "getPhonebookEntries error:" << rasErrorString(res);
        }
    }

    // Copy phonebook entries to return value
    for (int i=0;i<(int)numEntries;i++){
        RASENTRYNAME entry = entries[i];
        retVal.append(QString::fromWCharArray(entry.szEntryName));
    }
    return retVal;
}

QHash<QString, QString> RasDriver::getDevices()
{
    QHash<QString, QString> retVal;
    RASDEVINFO* entries = new RASDEVINFO[1];
    entries[0].dwSize = sizeof(RASDEVINFO);
    DWORD numEntries;
    DWORD size = entries[0].dwSize;
    DWORD res = RasEnumDevices(entries, &size, &numEntries);

    // More than one device info
    if (res == ERROR_BUFFER_TOO_SMALL) {
        // Allocate enough memory to get
        // all the device info entries.
        delete[] entries;
        entries = new RASDEVINFO[numEntries];
        entries[0].dwSize = sizeof(RASDEVINFO);
        res = RasEnumDevices(entries, &size, &numEntries);
        if (res) {
            qDebug() << "getDevices error:" << rasErrorString(res);
        }
    }

    // Copy device infos to return value
    for (int i=0;i<(int)numEntries;i++){
        RASDEVINFO entry = entries[i];
        retVal.insert(QString::fromWCharArray(entry.szDeviceName), QString::fromWCharArray(entry.szDeviceType));
    }
    return retVal;

}
QList<RASCONN> RasDriver::getActiveConnections()
{
    QList<RASCONN> retVal;
    QHash<QString, RASCONN> connections = getConnections();

    if(!connections.isEmpty()){
        foreach(RASCONN connection, connections.values()){
            // Get the connection status.
            RASCONNSTATUS status;
            status.dwSize = sizeof(status);
            int res = RasGetConnectStatus(
                        connection.hrasconn, &status);
            if (res) {
                // Error. Report it.
                qDebug() << "getActiveConnections error:" << rasErrorString(res);
            } else {
                // Found connection, show details.
                if (status.rasconnstate
                        == RASCS_Connected) {
                    qDebug() << "Device type: " +
                                QString::fromWCharArray(status.szDeviceType);
                    qDebug() << "Device name: " +
                                QString::fromWCharArray(status.szDeviceName);
                    qDebug() << "Connected to: " +
                                QString::fromWCharArray(connection.szEntryName);
                    retVal.append(connection);
                } else {
                    // A connection was detected but its
                    // status is RASCS_Disconnected.
                    qDebug() << ("Connection Error");
                }
            }
        }
    }
    return retVal;
}

QHash<QString, RASCONN> RasDriver::getConnections()
{
    QHash<QString, RASCONN> retVal;
    RASCONN* rcs = new RASCONN[1];
    rcs[0].dwSize = sizeof(RASCONN);
    DWORD numConns = 0;
    DWORD size = rcs[0].dwSize;
    // Enumerate the connections.
    DWORD res = RasEnumConnections(rcs, &size, &numConns);
    if (numConns > 1){
        delete[] rcs;
        rcs = new RASCONN[numConns];
        rcs[0].dwSize = sizeof(RASCONN);
        res = RasEnumConnections(rcs, &size, &numConns);
        if (res) {
            qDebug() << "getConnections multi error:" << rasErrorString(res);
        }
    }
    if (res) {
        // Error. Report it.
        qDebug() << "getConnections one error:" << rasErrorString(res);
    }
    for(int connNo = 0; connNo < numConns; connNo++){
        QString entryname = QString::fromStdWString(rcs[connNo].szEntryName);
        retVal.insert(entryname, rcs[connNo]);
    }
    return retVal;
}

RasDriver *RasDriver::connect(QString com)
{
    RasDriver *retVal = NULL;
    QString tapiDeviceName = getTapiByCOM().value(com);
    QString jcEntry = "JustConnect #1";
    QList<int> activeEntries;
    if(!tapiDeviceName.isEmpty()){

        // Find active JustConnect phonebook entries
        foreach(RASCONN entry, getActiveConnections()){
            QString entryName = QString::fromStdWString(entry.szEntryName);
            if(entryName.startsWith("JustConnect")){
                int no = entryName.split("#").at(1).toInt();
                activeEntries.append(no);
            }
        }
        if(!activeEntries.isEmpty()){

            // Find first free entry id
            qSort(activeEntries);
            int free = 1;
            foreach (int entryId, activeEntries) {
                if(entryId != free){
                    break;
                }
                free++;
            }
            jcEntry = "JustConnect #" + QString::number(free);
        }

        // Save RAS entry and connect it to TAPI device
        RASENTRY rasEntry;
        memset(&rasEntry, 0, sizeof(rasEntry));
        rasEntry.dwSize = sizeof(rasEntry);
        rasEntry.dwfOptions = RASEO_RequireCHAP | RASEO_RemoteDefaultGateway;
        rasEntry.dwfOptions2 = RASEO2_SecureClientForMSNet;
        rasEntry.dwfNetProtocols = RASNP_Ip;
        QString phoneNo = "*99#";
        phoneNo.toWCharArray(rasEntry.szLocalPhoneNumber);
        getDevices().value(tapiDeviceName).toWCharArray(rasEntry.szDeviceType);
        tapiDeviceName.toWCharArray(rasEntry.szDeviceName);

        // Check RAS entry save status
        DWORD dwResult = RasSetEntryProperties(NULL, (LPTSTR)jcEntry.toStdWString().c_str(), &rasEntry, sizeof(rasEntry), NULL, 0);
        if (dwResult == 0) {
            RASDIALPARAMS params;
            memset(&params, 0, sizeof(params));
            params.dwSize = sizeof(params);
            jcEntry.toWCharArray(params.szEntryName);
            //            QString domain = "*";
            //            domain.toWCharArray(params.szDomain);
            RASCONN connection = getConnections().value(jcEntry);
            //RasDriver *driver = new RasDriver();
            // Dial.
            dwResult = RasDial(
                        0, 0,
                        &params, 1,
                        (LPVOID)RasDriver::RasCallback,
                        &connection.hrasconn);
            if (dwResult) {
                qDebug() << "RasDial failed with code:" << rasErrorString(dwResult);
            }
        } else {
            qDebug() << "RasSetEntryProperties failed with code:" << QString::number(dwResult, 16);
        }
    } else {
        qDebug() << "No TAPI devices conneted to" << com;
    }

    return retVal;
}

void RasDriver::createPhonebookEntry(QString name)
{
    RASENTRY entry;
    QString phNumber = "*99#";
    phNumber.toWCharArray(entry.szLocalPhoneNumber);
}

QHash<QString, QString> RasDriver::getTapiByCOM()
{
    QHash<QString, QString> retVal;

    // Initialize TAPI lineApp
    HLINEAPP hLineApp;
    DWORD dwDevNum;
    DWORD dwApiVersion = TAPI_CURRENT_VERSION;
    LINEINITIALIZEEXPARAMS lineParams;;
    memset(&lineParams, 0, sizeof(LINEINITIALIZEEXPARAMS));
    lineParams.dwTotalSize = sizeof(LINEINITIALIZEEXPARAMS);
    lineParams.dwOptions = LINEINITIALIZEEXOPTION_USEEVENT;
    LONG errCode = lineInitializeEx(&hLineApp,NULL,NULL,NULL,
                                    &dwDevNum,&dwApiVersion,&lineParams);

    if(errCode == 0){

        // Iterate over TAPI lines
        for(uint i = 0; i < dwDevNum; i++){

            // Negotiate TAPI version
            LINEEXTENSIONID ExtensionID;
            DWORD dwNegApiVersion;
            HLINE hLine;
            errCode = lineNegotiateAPIVersion(
                        hLineApp,
                        i, 0x00010000,
                        TAPI_CURRENT_VERSION,
                        &dwNegApiVersion,
                        &ExtensionID
                        );

            // Open TAPI line
            errCode = lineOpen(
                        hLineApp,
                        i, &hLine,
                        dwNegApiVersion,
                        0, 0,
                        LINECALLPRIVILEGE_NONE,
                        0, NULL
                        );

            if(errCode == 0){

                // Get line caps and resolve the name
                LINEDEVCAPS *LineAddressC;
                LineAddressC = reinterpret_cast<LINEDEVCAPS *>(malloc( 1024 ));
                LineAddressC->dwTotalSize  = 1024;
                errCode = lineGetDevCaps(
                            hLineApp,
                            i,
                            dwNegApiVersion,
                            0,
                            LineAddressC
                            );

                // Calculate address of LineName
                char *lineNameAddr = reinterpret_cast<char *>(LineAddressC) + LineAddressC->dwLineNameOffset;
                QString lineName = QString::fromUtf16 (reinterpret_cast<ushort *>(lineNameAddr));
                free(LineAddressC);

                // Obtain com port name if possible
                VARSTRING *pvarStrDevID = reinterpret_cast<VARSTRING *>(malloc(sizeof(VARSTRING)+255));
                QString className;
                memset(pvarStrDevID,0,sizeof(VARSTRING)+255);
                pvarStrDevID->dwTotalSize = sizeof(VARSTRING)+255;

                className = "comm/datamodem/portname";
                errCode = lineGetID(
                            hLine,0,0,
                            LINECALLSELECT_LINE,
                            pvarStrDevID,
                            className.toStdWString().c_str());

                if(errCode == 0) {
                    QString com(reinterpret_cast<char *>(pvarStrDevID) + pvarStrDevID->dwStringOffset);
                    retVal.insert(com, lineName);
                }
                free(pvarStrDevID);
                lineClose(hLine);
            }
        }
    } else {
        qDebug() << "TAPI init error:" << QString::number(errCode,16);
    }
    return retVal;
}

QString RasDriver::rasErrorString(UINT errCode)
{
    QString errTxt = "";
    if (errCode) {
        char buff[256];
        RasGetErrorString(
                    errCode, buff, sizeof(buff));
        errTxt = QString::fromUtf16((ushort *)buff);
    }
    return errTxt;
}
void WINAPI RasDriver::RasCallback(HRASCONN hrasconn,
                                   UINT unMsg, RASCONNSTATE rascs,
                                   DWORD dwError, DWORD dwExtendedError)
{
    QString S = "";
    if (dwError) {
        // Error occurred, show the error string.
        char buff[256];
        RasGetErrorString(
                    dwError, buff, sizeof(buff));
        QString error = QString::fromUtf16((ushort *)buff);
        qDebug() << "RasCallback error:" << error;
        return;
    }
    switch (rascs) {
    case RASCS_OpenPort:
        break;
    case RASCS_ConnectDevice:
        break;
    case RASCS_AllDevicesConnected:
        break;
    case RASCS_AuthNotify:
        break;
    case RASCS_AuthRetry:
        break;
    case RASCS_AuthCallback:
        break;
    case RASCS_AuthChangePassword:
        break;
    case RASCS_AuthProject:
        break;
    case RASCS_AuthLinkSpeed:
        break;
    case RASCS_AuthAck:
        break;
    case RASCS_ReAuthenticate:
        break;
    case RASCS_PrepareForCallback:
        break;
    case RASCS_WaitForModemReset:
        break;
    case RASCS_WaitForCallback:
        break;
    case RASCS_Projected:
        break;
    case RASCS_StartAuthentication:
        break;
    case RASCS_CallbackComplete:
        break;
    case RASCS_LogonNetwork:
        break;
    case RASCS_SubEntryConnected:
        break;
    case RASCS_SubEntryDisconnected:
        break;
    case RASCS_Interactive:
        break;
    case RASCS_RetryAuthentication:
        break;
    case RASCS_CallbackSetByCaller:
        break;
    case RASCS_PasswordExpired:
        break;
    case RASCS_InvokeEapUI:
        break;
        // Build a status string based on the
        // status message.
    case RASCS_PortOpened :
        S = "Port opened..."; break;
    case RASCS_DeviceConnected :
        S = "Connected..."; break;
    case RASCS_Authenticate :
        S = "Logging on..."; break;
    case RASCS_Authenticated :
        S = "Authenticated"; break;
    case RASCS_Connected : {
        S = "Logon Complete";
        break;
    }
    case RASCS_Disconnected :
        S = "Disconnected"; break;
    }
    // Show the status message in the memo.
    if (S != "")
        qDebug() << "RAS message:" << S;
}

bool RasDriver::initialized = false;

QLibrary *RasDriver::rasDll = NULL;
_RasEnumEntries RasDriver::RasEnumEntries;
_RasGetErrorString RasDriver::RasGetErrorString;
_RasEnumConnections RasDriver::RasEnumConnections;
_RasGetConnectStatus RasDriver::RasGetConnectStatus;
_RasSetEntryProperties RasDriver::RasSetEntryProperties;
_RasEnumDevices RasDriver::RasEnumDevices;
_RasDial RasDriver::RasDial;

QLibrary *RasDriver::tapiDll = NULL;
_lineInitializeEx RasDriver::lineInitializeEx;
_lineNegotiateAPIVersion RasDriver::lineNegotiateAPIVersion;
_lineOpen RasDriver::lineOpen;
_lineGetID RasDriver::lineGetID;
_lineGetDevCaps RasDriver::lineGetDevCaps;
_lineClose RasDriver::lineClose;

