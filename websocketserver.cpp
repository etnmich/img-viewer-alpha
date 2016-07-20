#include "websocketserver.h"

WebsocketServer::WebsocketServer(qint16 port, QObject *parent) : QObject(parent)
{
    server = new QWebSocketServer("Image Receiver", QWebSocketServer::NonSecureMode, this);
    if (server->listen(QHostAddress::Any, port)) {
        //do things
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    } else {
        throw Error::ListenFailed;
    }
}

WebsocketServer::~WebsocketServer()
{
    server->close();
    qDeleteAll(clients);
}

void WebsocketServer::newConnection()
{
    QWebSocket *client = server->nextPendingConnection();
    clients.append(client);
    //connects. TBA
    connect(client, SIGNAL(textMessageReceived(QString)), this, SLOT(processTextMessage(QString)));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
}

void WebsocketServer::clientDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket*>(sender());
    clients.removeAll(client);
    client->deleteLater();
}

void WebsocketServer::processTextMessage(const QString &message)
{
    // do things, gonna use json for this

    //don't need this yet
    //QWebSocket *client = qobject_cast<QWebSocket*>(sender());
    QJsonObject obj = QJsonDocument::fromJson(message.toUtf8()).object();
    if (!obj.contains("type") || !obj.contains("url")) {
        // logging for dbg
        qDebug() << obj;
        return;
        // this is not the json we're looking for
    }
    QString type = obj.value("type").toString();
    QString url = obj.value("url").toString();
    if (type == "img") {
        emit ImageUrlReceived(url);
    }
    // TODO: add more handlers
    // elseif (...) {...}
    else {
        // log this for test purposes
        qDebug() << obj.value("type") << obj.value("url");
    }
    //emit UrlReceived(message);
}
