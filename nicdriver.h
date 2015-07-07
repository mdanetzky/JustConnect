#ifndef NICDRIVER_H
#define NICDRIVER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QStringList>
#include <QTextCodec>

class NicDriver: public QObject
{
    Q_OBJECT

public:
    NicDriver();
    static NicDriver *getDriverByIMEI(QString imei);
    static QHash<QString, NicDriver *> nicsByImei;

    bool connect();
    bool connectNetsh();
    bool isConnectedNetsh();
    QString getImei() const;
    void setImei(const QString &value);

    QString getNicName() const;
    void setNicName(const QString &value);

    QString getProfileName() const;
    void setProfileName(const QString &value);

    QStringList getProfileNames() const;
    void setProfileNames(const QStringList &value);

private:
    QString imei;
    QString nicName;
    QString profileName;
    QStringList profileNames;
    static QString netshExePath;
    static QTextCodec *cmdCodec;
};

#endif // NICDRIVER_H
