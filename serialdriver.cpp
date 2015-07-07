#include "serialdriver.h"
#include <QRegExp>
#include <QThread>

SerialDriver::SerialDriver(){}

QList<QSerialPortInfo> SerialDriver::findDevices() {
    QList<QSerialPortInfo> retVal;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite)){
            serial.close();
            retVal.append(info);
        }
    }
    return retVal;
}

QString SerialDriver::sendAT(QSerialPortInfo *info, QString data){

    QSerialPort *serial = new QSerialPort(QSerialPortInfo(*info));
    QByteArray buffer;
    QString retVal = "";
    QByteArray sendBuffer;

    qDebug() << "Opening COM:" << info->portName();
    if (serial->open(QIODevice::ReadWrite)) {

        // Try to set basics on the port
//        serial->setParity(QSerialPort::NoParity);
//        serial->setFlowControl(QSerialPort::SoftwareControl);
//        serial->setDataBits(QSerialPort::Data8);
//        serial->setStopBits(QSerialPort::OneStop);
//        serial->setDataTerminalReady(true);

        int retry = 0;
        int maxRetries = 5;

        // Wait for the port to get ready
        serial->waitForReadyRead(1000);

        // Split commands delimited by ;
        foreach (QString cmd, data.split(";")) {

            // Append CR to command if not present
            if(!cmd.endsWith("\r")){
                cmd += "\r";
            }

            // Convert String to buffer
            sendBuffer = cmd.toStdString().c_str();
            qDebug() << info->portName() << "sending :" << cmd.trimmed();

            // Send command
            serial->write(sendBuffer);
            serial->waitForBytesWritten(1000);

            // Read the answer from device
            forever {
                serial->waitForReadyRead(1000);
                buffer = serial->readAll();
                if(!buffer.isEmpty()){

                    // If echo, then continue reading
                    if(buffer == cmd){
                        //qDebug() << info->portName() << "received echo, keep reading";
                        serial->waitForReadyRead(500);
                        buffer = serial->readAll();
                    }
                    retVal += QString(buffer);
                    // qDebug() << "Serial received buffer:" << retVal;

                    // remove all echoes
                    retVal.replace(cmd, "");
                    break;
                }

                // If max retries reached - finish
                if(retry == maxRetries){
                    break;
                }
                retry++;
                //qDebug() << info->portName() << "retry";
            }
        }

        serial->close();

        // Replace all new lines with single CRs
        retVal = retVal.replace("\n", "\r").trimmed();
        retVal = retVal.replace(QRegExp("\r+"), "\r");
        qDebug() << info->portName() << "Serial received:" << retVal;
    } else {
        qDebug() << info->portName() << "Serial err :" << serial->errorString();
        qDebug() << info->portName() << "Serial err :" << serial->error();
        serial->close();
    }

    // Cleanup
    delete serial;

    // Return received answer
    return retVal;
}
