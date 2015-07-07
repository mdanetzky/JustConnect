#ifndef APPLICATIONSKILLER_H
#define APPLICATIONSKILLER_H

#include "applicationskillerworker.h"
#include <QObject>

class ApplicationsKiller : public QObject
{
    Q_OBJECT

public:
    static ApplicationsKillerWorker *startKillerThread();

};

#endif // APPLICATIONSKILLER_H
