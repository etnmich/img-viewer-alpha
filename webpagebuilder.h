#ifndef WEBPAGEBUILDER_H
#define WEBPAGEBUILDER_H

#include <QObject>
#include <QUrl>

class WebPageBuilder : public QObject
{
    Q_OBJECT
public:
    explicit WebPageBuilder(QObject *parent = 0);

    void setSource(const QUrl &source);
    void setBgColor(const int &color);
signals:

public slots:

    QByteArray buildPage() const;
    void resetRotation();
    void rotateClockwise();
    void rotateCounterCw();
private:
    QString baseStart, baseMid1, baseMid2, baseEnd, bgColor, src;
    int rotation;
};

#endif // WEBPAGEBUILDER_H
