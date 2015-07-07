#ifndef SETTINGSDBUTILS_H
#define SETTINGSDBUTILS_H

#include <QHash>
#include <QString>

class SettingsDBUtils
{
public:
    static QHash<QString, QHash<QString, QString> *> parseSettings(QString iniFileName);
};

#endif // SETTINGSDBUTILS_H
