#include "widget.h"
#include <QApplication>
#include <QThread>
#include <QtCore/QDebug>
#include <QSettings>
#include "serialdriver.h"
#include "tests.h"
#include "justconnect.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);
    JustConnect justConnect;
    justConnect.init();
    //Widget w;

    //Tests::testPatternParser();
    //Tests::testPing();
    //Tests::testContinuousPing();
    //Tests::testTrayMessage();

    //w.show();
    int retVal = a.exec();
    UITrayIcon::remove();
    return retVal;
}
