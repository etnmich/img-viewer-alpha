#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
//#include <QList>

class WebsocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebsocketServer(qint16 port, QObject *parent = 0);

    enum class Error {
        ListenFailed
    };

    ~WebsocketServer();
signals:
    void ImageUrlReceived(const QString &url);

public slots:

private:
    QWebSocketServer *server;
    QList<QWebSocket*> clients;

private slots:
    void newConnection();
    void clientDisconnected();
    void processTextMessage(const QString &message);
};

#endif // WEBSOCKETSERVER_H
