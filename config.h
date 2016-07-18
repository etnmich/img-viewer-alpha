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
        RWaccessError
    };

signals:

public slots:

private:
    QFile *file;
    QMap<QString,QString> data;
};

#endif // CONFIG_H
