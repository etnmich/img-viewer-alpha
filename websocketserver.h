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
    explicit WebsocketServer(const QString &name, const bool &useSSL = false, QObject *parent = 0);
    ~WebsocketServer();
    bool isOnline() const;
    bool start(const quint16 &port);
    void stop();
    QString error();
signals:
    void ServerOnline(const bool&);
    void ImageUrlReceived(const QString&);
    void serverMsg(const QString&);
    void RotateLeftReceived();
    void RotateRightReceived();

public slots:

private:
    QWebSocketServer *server;
    QList<QWebSocket*> clients;
    QString errMsg;

private slots:
    void newConnection();
    void clientDisconnected();
    void processTextMessage(const QString &message);
};

#endif // WEBSOCKETSERVER_H
