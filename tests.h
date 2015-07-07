#ifndef TESTS_H
#define TESTS_H
#include <QDebug>
#include "patternparser.h"
#include <QHash>
#include "netmonitor.h"
#include "uitrayicon.h"

class Tests
{
public:
    static void testPatternParser();
    static void testPing();
    static void testContinuousPing();
    static void testTrayMessage();
    static void dumpHashSS(QHash<QString, QString> hashSS);
    static void dumpListS(QList<QString> listS);
};

#endif // TESTS_H
