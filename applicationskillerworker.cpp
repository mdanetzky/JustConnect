#include "applicationskillerworker.h"
#include <QProcess>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>

ApplicationsKillerWorker::ApplicationsKillerWorker()
{
    initialized = false;
}
void ApplicationsKillerWorker::init()
{
    if(initialized){
        return;
    }
    initialized = true;

    // TODO check if ini file exists
    QFile inputFile(":/MobileApplications.txt");

    // Initialize list of programs to kill
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            if(!line.isEmpty()){
               programNames.append(line);
            }
        }
        inputFile.close();
    }

}
void ApplicationsKillerWorker::process()
{
    QProcess *process;

    init();
    // Check and kill all programs from the list every sec.

    if(!programNames.isEmpty()){
        // Prepare QProcess
        process = new QProcess();
        process->setReadChannel(QProcess::StandardOutput);
        process->setReadChannelMode(QProcess::MergedChannels);

        // Read the list of currently running processes
        process->start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption");
        process->waitForStarted(1000);
        process->waitForFinished(1000);
        QString processList = "\n" + QString(process->readAll());
        process->close();

        // Chek if list contains any programnames to kill
        foreach(QString processTokill, programNames){
            if(processList.indexOf("\n" + processTokill) != -1){
                process->start("taskkill /F /IM \"" + processTokill + "\"");
                process->waitForStarted(3000);
                process->waitForFinished(3000);
                process->close();

                // TODO check the exit status of taskkill

                emit processKilled(processTokill);
            }
        }
        delete process;
        QTimer::singleShot(1000, this, SLOT(process()));
        return;
    }
    emit finished();
}

