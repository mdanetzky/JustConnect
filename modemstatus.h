#ifndef MODEMSTATUS_H
#define MODEMSTATUS_H

#include <QHash>
#include <QString>

class ModemStatus
{
public:
    ModemStatus();
    ModemStatus(int status, int domain, int roaming, int mode, int simState);
    void set(QHash<QString, QString> data);
    int getStatus();
    int getDomain();
    int getRoaming();
    int getMode();
    int getSimState();
private:
    int status;
    int domain;
    int roaming;
    int mode;
    int simState;
};

#endif // MODEMSTATUS_H
