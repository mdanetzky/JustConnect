#ifndef PATTERNPARSER_H
#define PATTERNPARSER_H

#include <QHash>
#include <QString>

class PatternParser
{
public:
    static QHash<QString, QString> parse(QString pattern, QString data);
    static QString fill(QString pattern, QHash<QString, QString> *data);
};

#endif // PATTERNPARSER_H
