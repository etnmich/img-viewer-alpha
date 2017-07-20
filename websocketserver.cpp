#include "websocketserver.h"

WebsocketServer::WebsocketServer(const QString &name, const bool &secure, QObject *parent) : QObject(parent)
{
    server = new QWebSocketServer(name, secure ? QWebSocketServer::SecureMode : QWebSocketServer::NonSecureMode, this);
}

WebsocketServer::~WebsocketServer()
{
    stop();
    qDeleteAll(clients);
}

bool WebsocketServer::isOnline() const
{
    return server->isListening();
}

bool WebsocketServer::start(const quint16 &port)
{
    if (server->listen(QHostAddress::Any, port)) {
        //do things
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        errMsg = "";
        emit serverMsg(QString("Server started succesfully on port %1").arg(port));
        emit ServerOnline(true);
        return true;
    } else {
        errMsg = server->errorString();
        emit serverMsg(QString("Server error: %1").arg(errMsg));
        return false;
    }
}

void WebsocketServer::stop() {
    server->close();
    emit ServerOnline(false);
    emit serverMsg(QString("Server has shut down"));
}

QString WebsocketServer::error()
{
    return errMsg;
}

void WebsocketServer::newConnection()
{
    qDebug() << "got connection";
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
    if (!obj.contains("type") || !obj.contains("payload")) {
        // logging for dbg
        qDebug() << obj;
        return;
        // this is not the json we're looking for
    }
    QString type = obj.value("type").toString();
    if (type == "img") {
        QString url = obj.value("payload").toString();
        emit ImageUrlReceived(url);
    }
    if (type == "rotate") {
        QString direction = obj.value("payload").toString();
        if (direction == "right") {
            emit RotateRightReceived();
        } else if (direction == "left") {
            emit RotateLeftReceived();
        } else {
            qDebug() << "received unknown rotation \"" << direction << '"';
        }
    }
    // TODO: add more handlers
    // elseif (...) {...}
    else {
        // log this for test purposes
        qDebug() << obj.value("type") << obj.value("url");
    }
    //emit UrlReceived(message);
}
