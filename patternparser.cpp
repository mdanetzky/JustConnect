#include "patternparser.h"

QHash<QString, QString> PatternParser::parse(QString pattern, QString data)
{
    QHash<QString, QString> retVal;
    QList<QStringRef> patternDelimiters;

    // TODO cache parsed patternDelimiters lists

    bool tokenfound = false;
    QStringRef toParse = pattern.midRef(0);
    QStringRef parsed = pattern.midRef(0,0);

    // TODO parse using split and regexp:
    //    QStringList delimitedStarts = pattern.split("{{", QString::KeepEmptyParts);
    //    foreach (QString elem, delimitedStarts) {
    //        qDebug() << elem;
    //    }

    forever{
        if(toParse.indexOf("{{") == -1 && toParse.indexOf("}}") == -1 ){
            if(!toParse.isEmpty()){
                patternDelimiters.append(toParse);
            }
            break;
        }
        if(tokenfound){
            parsed = toParse.mid(0, toParse.indexOf("}}"));
            patternDelimiters.append(parsed);
            toParse = toParse.mid(parsed.length() + 2);
            tokenfound = false;
        } else {
            parsed = toParse.mid(0, toParse.indexOf("{{"));
            patternDelimiters.append(parsed);
            toParse = toParse.mid(parsed.length() + 2);
            tokenfound = true;
        }
    }
    //    qDebug() << "patternDelimiters : " << patternDelimiters;

    bool filler = true;
    //    QStringRef toParse = data.midRef(0);
    //    QStringRef parsed;
    toParse = data.midRef(0);
    parsed = data.midRef(0,0);
    QString tokenName = "";
    foreach (QStringRef elem, patternDelimiters) {
        if(filler){
            int pos = toParse.indexOf(elem);
            parsed = toParse.mid(0, pos);
            toParse = toParse.mid(pos + elem.length());
            filler = false;
            if(!tokenName.isEmpty()){
                retVal.insert(tokenName, parsed.toString());
            }
        } else {
            tokenName = elem.toString().toLower();
            filler = true;
        }

    }
    // if last token hasn't been filled -> put the rest of data there
    if(!tokenName.isEmpty() && !retVal.contains(tokenName)){
        retVal.insert(tokenName, toParse.toString());
    }
    // qDebug() << retVal;
    return retVal;

}

QString PatternParser::fill(QString pattern, QHash<QString, QString> *data)
{
    QString retVal = pattern;
    QHash<QString, QString>::iterator dataEntry;
    for(dataEntry = data->begin();
        dataEntry != data->end();
        dataEntry++){
        retVal = retVal.replace("{{" + dataEntry.key() + "}}", dataEntry.value());
    }
    return retVal;
}
