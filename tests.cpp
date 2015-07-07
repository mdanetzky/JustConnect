#include "tests.h"

void Tests::testPatternParser()
{
    QString pattern;
    QString data;

    qDebug() << "";
    qDebug() << "Begin test: testBracketParser";
    qDebug() << "";
    pattern = ",{{aa}},{{bb}},";
    data = ",1,2,";
    qDebug() << "pattern: " << pattern;
    qDebug() << "data   : " << data;
    qDebug() << "parsed : " << PatternParser::parse(pattern, data);

    pattern = "{{aa}},{{bb}}";
    data = "1,2";
    qDebug() << "pattern: " << pattern;
    qDebug() << "data   : " << data;
    qDebug() << "parsed : " << PatternParser::parse(pattern, data);

    qDebug() << "";
    qDebug() << "End test: testPatternParser";
    qDebug() << "";

}

void Tests::testPing()
{
//    NetTest ping;
    qDebug() << "ping test 8.8.8.8 success:" << NetMonitor::testTcpReachable("8.8.8.8", 53);
}

void Tests::testContinuousPing()
{
    NetMonitor netmon;
    netmon.startNetMonitoring();
    qDebug() << "Cont pinging started";
}

void Tests::testTrayMessage()
{
    UITrayIcon::setIcon(":/antenna_with_bars.svg");
    UITrayIcon::showMessage("aa");
}

void Tests::dumpHashSS(QHash<QString, QString> hashSS)
{
    foreach (QString key, hashSS.keys()) {
        qDebug() << key << " => " << hashSS.value(key);
    }
}

void Tests::dumpListS(QList<QString> listS)
{
    foreach (QString key, listS) {
        qDebug() << key;
    }
}
