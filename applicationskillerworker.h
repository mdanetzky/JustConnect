#ifndef APPLICATIONSKILLERWORKER_H
#define APPLICATIONSKILLERWORKER_H

#include <QObject>
#include <QStringList>
#include <QTimer>

class ApplicationsKillerWorker: public QObject
{
    Q_OBJECT
public:
    ApplicationsKillerWorker();

public slots:
    void process();

signals:
    void processKilled(QString);
    void finished();
private:
    QStringList programNames;
    bool initialized;
    void init();
};

#endif // APPLICATIONSKILLERWORKER_H
