#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFile>
#include <QMap>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(const QString &filename, QObject *parent = 0);
    enum class Error {
        RWaccessError,
        ParseError,
        KeyNotFound,
        SaveError
    };

    QVariant getKey(const QString &key) const;
    void setKey(const QString &key, const QVariant &value);
signals:

public slots:

    void save();
private:
    QFile *file;
    QVariantMap data;
    QByteArray makeConfigFile() const;
};

#endif // CONFIG_H
