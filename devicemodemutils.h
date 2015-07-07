#ifndef DEVICEMODEMUTILS_H
#define DEVICEMODEMUTILS_H
#include <QHash>
#include <QSerialPortInfo>
#include <QString>
#include "devicemodeminfo.h"
#include "devicemodem.h"
#include "serialdriver.h"
#include "patternparser.h"

typedef QHash<QString, QString> DeviceInfoHash;

/*!
 * \brief The DeviceModemUtils class
 *
 * Basic modem utilities.
 *
 * \author Matthias Danetzky
 */
class DeviceModemUtils
{
public:
    /*!
     * \brief Retrieves and parses modem information using serial connection
     * \param port
     * \param modem definitions
     * \return parsed fieldName, value pairs
     */
    static QHash<QString, QString> getDeviceInfo(QSerialPortInfo *port, DeviceModemInfo *modem);
    static bool initModem(QSerialPortInfo *port, DeviceModemInfo *modem);
    static QString portSignature(QSerialPortInfo port);
    static QString portSignaturePtr(QSerialPortInfo *port);
    static bool hasPortSignature(DeviceModem *modem, QString portSig);
};

#endif // DEVICEMODEMUTILS_H
