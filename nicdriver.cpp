#include "nicdriver.h"
#include <QProcess>
#include <QRegExp>
#include <QFile>
#include <QDebug>
#include <QLocale>
#include <QThread>
#include "windows.h"

NicDriver::NicDriver()
{
}

NicDriver *NicDriver::getDriverByIMEI(QString imei)
{
    if(netshExePath.isEmpty()){
        // Find the right netsh path
        QString winpath = getenv("windir");
        netshExePath = winpath + "\\SysNative\\" + "netsh.exe";
        QFile netshExe(netshExePath);
        if(!netshExe.exists())
        {
            netshExePath = winpath + "\\System32\\" + "netsh.exe";
        }

        // Find shell code page and prepare text codec
        int cp = GetOEMCP();
        QString cpStr = QString::number(cp);
        QByteArray ba = cpStr.toLatin1();
        cmdCodec = &*QTextCodec::codecForName(ba.constData());
    }


    // Define process to run shell commands
    QProcess *process = new QProcess();
    process->setReadChannel(QProcess::StandardOutput);
    process->setReadChannelMode(QProcess::MergedChannels);

    // Read the list of currently present interfaces
    process->start(netshExePath + " mbn show interfaces");
    process->waitForStarted(1000);
    process->waitForFinished(3000);
    QByteArray arr = process->readAll();
    QString interfacesOut = cmdCodec->toUnicode(arr);
    QStringList interfacesLines = interfacesOut.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    process->close();

    QRegExp nameRgx("[ \t]+name[ \t]+:.+",Qt::CaseInsensitive);
    QRegExp imeiRgx("[ \t]+device id[ \t]+:.+",Qt::CaseInsensitive);
    QString newNicName = "";
    QString newImei = "";
    QHash<QString, NicDriver *> tmpnNicByImei;

    // Parse interfaces list from netsh
    foreach(QString line, interfacesLines){
        if(nameRgx.exactMatch(line)){
            newNicName = line.split(":").at(1).trimmed();
            qDebug() << newNicName;
        }
        if(imeiRgx.exactMatch(line)){
            newImei = line.split(":").at(1).trimmed();
            qDebug() << newImei;
        }
        // Broadband interface found
        if(!newNicName.isEmpty() && !newImei.isEmpty()){
            // Find broadband profiles
            process->start(netshExePath + " mbn show profiles interface=\"" + newNicName + "\"");
            process->waitForStarted(1000);
            process->waitForFinished(3000);
            QString profilesOut = cmdCodec->toUnicode(process->readAll());
            process->close();
            QStringList profilesLines = profilesOut.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
            QRegExp profilesBeginRgx("----[-]+",Qt::CaseInsensitive);
            QStringList profiles;
            bool profilesStarted = false;
            foreach (QString line, profilesLines) {
                line = line.trimmed();
                if(profilesStarted && !line.isEmpty()){
                    profiles.append(line);
                    qDebug() << "Profile found" << line;
                }
                if(profilesBeginRgx.exactMatch(line)){
                    profilesStarted = true;
                }
            }
            if(!profiles.isEmpty()){
                // Create driver
                NicDriver *driver = new NicDriver();
                driver->setImei(newImei);
                driver->setNicName(newNicName);
                driver->setProfileNames(profiles);
                tmpnNicByImei.insert(newImei, driver);
                qDebug() << "Driver found :"<<newImei;
            }
            //qDebug() << interfacesOut;
            newImei = "";
            newNicName = "";
        }
        // qDebug() << line;
    }
    foreach (QString currentImei, nicsByImei.keys()) {
        if(!tmpnNicByImei.contains(currentImei)){
            delete nicsByImei.value(currentImei);
            nicsByImei.remove(currentImei);
        }
    }
    foreach (QString newImei, tmpnNicByImei.keys()) {
        if(!nicsByImei.contains(newImei)){
            // New NIC found
            nicsByImei.insert(newImei, tmpnNicByImei.value(newImei));
            qDebug() << "NIC driver added, imei:" << newImei;
        } else {
            // Remove unused driver
            delete tmpnNicByImei.value(newImei);
        }
    }

    // Cleanup
    delete process;

    return nicsByImei.value(imei, NULL);
}

bool NicDriver::connect()
{
    connectNetsh();
}

bool NicDriver::connectNetsh()
{
    QProcess *process = new QProcess();
    process->setReadChannel(QProcess::StandardOutput);
    process->setReadChannelMode(QProcess::MergedChannels);
    setProfileName(getProfileNames().first());
    QString connectLine = netshExePath + " mbn connect interface=\"" + getNicName() +
            "\" connmode=name name=\"" + getProfileName() + "\"";
    qDebug() << "Netsh connect:" << connectLine;
    process->start(connectLine);
    process->waitForStarted(3000);
    process->waitForFinished(5000);
    QByteArray arr = process->readAll();
    QString interfacesOut = cmdCodec->toUnicode(arr);
    qDebug() << interfacesOut;
    int esss = process->exitStatus();
    qDebug() << "netsh exit status:" << esss;
    int exitCode = process->exitCode();
    qDebug() << "netsh exit code:" << exitCode;
    if(exitCode != 0){
        // Retry
        QThread::msleep(2000);
        process->start(connectLine);
        process->waitForStarted(3000);
        process->waitForFinished(5000);
        arr = process->readAll();
        interfacesOut = cmdCodec->toUnicode(arr);
        qDebug() << interfacesOut;
        esss = process->exitStatus();
        qDebug() << "netsh exit status:" << esss;
        exitCode = process->exitCode();
        qDebug() << "netsh exit code:" << exitCode;
    }
    process->close();
    delete process;
    return (exitCode == 0);
}

bool NicDriver::isConnectedNetsh()
{
    QProcess *process = new QProcess();
    process->setReadChannel(QProcess::StandardOutput);
    process->setReadChannelMode(QProcess::MergedChannels);
    setProfileName(getProfileNames().first());
    QString connectLine = netshExePath + " mbn connect interface=\"" + getNicName() +
            "\" connmode=name name=\"" + getProfileName() + "\"";
    qDebug() << "Netsh connect:" << connectLine;
    process->start(connectLine);
    process->waitForStarted(3000);
    process->waitForFinished(5000);
    QByteArray arr = process->readAll();
    QString interfacesOut = cmdCodec->toUnicode(arr);
    qDebug() << interfacesOut;
    int exitCode = process->exitCode();
    process->close();
    delete process;
    return (exitCode == 0);
}
QString NicDriver::getImei() const
{
    return imei;
}

void NicDriver::setImei(const QString &value)
{
    imei = value;
}
QString NicDriver::getNicName() const
{
    return nicName;
}

void NicDriver::setNicName(const QString &value)
{
    nicName = value;
}
QString NicDriver::getProfileName() const
{
    return profileName;
}

void NicDriver::setProfileName(const QString &value)
{
    profileName = value;
}
QStringList NicDriver::getProfileNames() const
{
    return profileNames;
}

void NicDriver::setProfileNames(const QStringList &value)
{
    profileNames = value;
}

QHash<QString, NicDriver*> NicDriver::nicsByImei;
QString NicDriver::netshExePath = "";
QTextCodec *NicDriver::cmdCodec = NULL;
