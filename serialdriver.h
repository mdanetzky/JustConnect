#ifndef SERIALDRIVER_H
#define SERIALDRIVER_H

#include <QList>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>

/*!
 * \brief The SerialDriver class
 *
 * Sends and receives single data communications via serial ports.
 *
 * \author Matthias Danetzky
 */
class SerialDriver
{
public:
    SerialDriver();
    static QList<QSerialPortInfo> findDevices();
    static QString sendAT(QSerialPortInfo *info, QString data);
};

#endif // SERIALDRIVER_H
