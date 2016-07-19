#include "config.h"

Config::Config(const QString &filename, QObject *parent) : QObject(parent)
{
    file = new QFile(filename, this);
    if (!file->open(QIODevice::ReadWrite)) {
        //error, schmerror. do something if failure.
        throw Error::RWaccessError;
    }
    // aaaa error handle-aink. cannot fathom ATM
    while (!file->atEnd()) {
        QString dataStr = file->readLine();
        // this will suck if things could have ':':s
        // will fix later
        QStringList splitData = dataStr.split(':');
        // error if more than 2 'fields' mby
        data.insert(splitData[0], splitData[1]);
    }
}

QString Config::getKey(const QString &key) const
{
    if (!data.contains(key)) {
        throw Error::KeyNotFound;
    } else {
        return data[key];
    }
}

void Config::setKey(const QString &key, const QString &value)
{
    // 2ez.
    data[key] = value;
}

void Config::save()
{
    file->reset();
    file->write(makeConfigFile());
    file->resize(file->pos());
}

QByteArray Config::makeConfigFile() const
{
    QByteArray conf;
    for (QMap<QString,QString>::const_iterator i = data.begin(); i != data.end(); i++) {
        conf.append(QString("%1:%2\n").arg(i.key(), i.value()));
    }
    return conf;
}

/*
So... sleepy.
Just list some TODO here.

- save (private)
- getKey (public)
- setKey (public)

> backup while save? bad things could happen if crashes during write
 > rename to (file).bak + create new file would bee cool
  > also check for backup if failure in open/parse

*/

