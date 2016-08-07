#include "config.h"

Config::Config(const QString &filename, QObject *parent) : QObject(parent)
{
    file = new QFile(filename, this);
    if (!file->open(QIODevice::ReadWrite)) {
        throw Error::RWaccessError;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file->read(file->size()));
    if (doc.isObject()) {
        data = doc.object().toVariantMap();
    } else {
        // do stuff
        //throw Error::ParseError;
    }
}

QVariant Config::getKey(const QString &key) const
{
    if (!data.contains(key)) {
        throw Error::KeyNotFound;
    } else {
        return data[key];
    }
}


void Config::setKey(const QString &key, const QVariant &value)
{
    // 2ez.
    data[key] = value;
}

void Config::save()
{
    file->reset();
    if (file->write(makeConfigFile()) < 0) {
        throw Error::SaveError;
    }
    file->resize(file->pos());
}

QByteArray Config::makeConfigFile() const
{
    QJsonDocument doc;
    doc.setObject(QJsonObject::fromVariantMap(data));


    return doc.toJson();
}
