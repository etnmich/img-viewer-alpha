#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFile>
#include <QMap>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(const QString &filename, QObject *parent = 0);
    enum class Error {
        RWaccessError,
        KeyNotFound,
        SaveError
    };

    QString getKey(const QString &key) const;
    void setKey(const QString &key, const QString &value);
signals:

public slots:

    void save();
private:
    QFile *file;
    QMap<QString,QString> data;
    QByteArray makeConfigFile() const;
};

#endif // CONFIG_H
