#include "modemstatus.h"

ModemStatus::ModemStatus(int status, int domain, int roaming, int mode, int simState)
{
    this->status = status;
    this->domain = domain;
    this->roaming = roaming;
    this->mode = mode;
    this->simState = simState;
}

ModemStatus::ModemStatus(){}

void ModemStatus::set(QHash<QString, QString> data)
{
    status = data.value("status", QString(status)).toInt();
    domain = data.value("domain", QString(status)).toInt();
    roaming = data.value("roaming", QString(status)).toInt();
    mode = data.value("mode", QString(status)).toInt();
    simState = data.value("simstate", QString(status)).toInt();
}
int ModemStatus::getStatus(){
    return status;
}

int ModemStatus::getDomain()
{
    return domain;
}

int ModemStatus::getRoaming()
{
    return roaming;
}

int ModemStatus::getMode()
{
    return mode;
}

int ModemStatus::getSimState()
{
    return simState;
}
