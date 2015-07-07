#ifndef RASDRIVERDEFS_H
#define RASDRIVERDEFS_H
#include "windows.h"
#include "ras.h"
#include "raserror.h"
#include "tapi.h"

// RAS bindings
typedef DWORD WINAPI (*_RasEnumEntries) (
        LPSTR reserved,
        LPSTR lpszPhonebook,
        LPRASENTRYNAME lprasentryname,
        LPDWORD lpcb,
        LPDWORD lpcEntries
        );

typedef DWORD WINAPI (*_RasGetErrorString) (
        UINT uErrorValue,
        LPSTR lpszErrorString,
        DWORD cBufSize
        );

typedef DWORD WINAPI (*_RasEnumConnections) (
        LPRASCONN lprasconn,
        LPDWORD lpcb,
        LPDWORD lpcConnections
        );

typedef DWORD WINAPI (*_RasGetConnectStatus) (
        HRASCONN hrasconn,
        LPRASCONNSTATUS lprasconnstatus
        );

typedef DWORD WINAPI (*_RasSetEntryProperties) (
        LPTSTR lpszPhonebook,
        LPTSTR lpszEntry,
        LPRASENTRY lpRasEntry,
        DWORD dwEntryInfoSize,
        LPBYTE lpbDeviceInfo,
        DWORD dwDeviceInfoSize
        );

typedef DWORD WINAPI (*_RasEnumDevices) (
        LPRASDEVINFO lpRasDevInfo,
        LPDWORD lpcb,
        LPDWORD lpcDevices
        );

typedef DWORD WINAPI (*_RasDial) (
        LPRASDIALEXTENSIONS lpRasDialExtensions,
        LPCTSTR lpszPhonebook,
        LPRASDIALPARAMS lpRasDialParams,
        DWORD dwNotifierType,
        LPVOID lpvNotifier,
        LPHRASCONN lphRasConn
        );

// TAPI bindings
typedef LONG WINAPI (*_lineInitializeEx) (
        LPHLINEAPP lphLineApp,
        HINSTANCE hInstance,
        LINECALLBACK lpfnCallback,
        LPCWSTR lpszFriendlyAppName,
        LPDWORD lpdwNumDevs,
        LPDWORD lpdwAPIVersion,
        LPLINEINITIALIZEEXPARAMS lpLineInitializeExParams
        );

typedef LONG WINAPI (*_lineNegotiateAPIVersion) (
        HLINEAPP hLineApp,
        DWORD dwDeviceID,
        DWORD dwAPILowVersion,
        DWORD dwAPIHighVersion,
        LPDWORD lpdwAPIVersion,
        LPLINEEXTENSIONID lpExtensionID
        );

typedef LONG WINAPI (*_lineOpen) (
        HLINEAPP hLineApp,
        DWORD dwDeviceID,
        LPHLINE lphLine,
        DWORD dwAPIVersion,
        DWORD dwExtVersion,
        DWORD_PTR dwCallbackInstance,
        DWORD dwPrivileges,
        DWORD dwMediaModes,
        LPLINECALLPARAMS const lpCallParams
        );

typedef LONG WINAPI (*_lineGetID) (
        HLINE hLine,
        DWORD dwAddressID,
        HCALL hCall,
        DWORD dwSelect,
        LPVARSTRING lpDeviceID,
        LPCWSTR lpszDeviceClass
        );

typedef LONG WINAPI (*_lineGetDevCaps) (
        HLINEAPP hLineApp,
        DWORD dwDeviceID,
        DWORD dwAPIVersion,
        DWORD dwExtVersion,
        LPLINEDEVCAPS lpLineDevCaps
        );

typedef LONG WINAPI (*_lineClose) (
        HLINE hLine
        );

#endif // RASDRIVERDEFS_H
