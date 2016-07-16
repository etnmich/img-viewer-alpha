#include "webpagebuilder.h"
#include "mainwindow.h"

// preprocessor definition for tis cat vomit, for debug reasons
#define logger(msg) dynamic_cast<MainWindow*>(this->parent())->LogMessage(QString("[webpagebuilder] " + msg))

WebPageBuilder::WebPageBuilder(QObject *parent) : QObject(parent)
{
    baseStart = "<html><head><title>Image Viewer template</title><style>body {background-color: ";
    bgColor = "black";
    baseMid1 = "; } img {max-height:100v%1; max-width:100v%2; margin: auto; position: absolute; top: 0; left: 0; bottom: 0; right: 0; -webkit-transform: rotate(";
    rotation = 0;
    baseMid2 = "deg);}</style></head><body><img id='output' src='";
    src = "data:image/gif;base64,R0lGODlhAQABAAD/ACwAAAAAAQABAAACADs=";
    baseEnd = "' /></body></html>";
}

void WebPageBuilder::setSource(const QUrl &source)
{
    // TODO: mby some user error noticing
    if (source == QUrl(""))
    {
        src = "data:image/gif;base64,R0lGODlhAQABAAD/ACwAAAAAAQABAAACADs=";
    }
    else
    {
        src = source.toString();
    }
    QString msg = "Source set to " + src;
    logger(msg);
    //dynamic_cast<MainWindow*>(this->parent())->LogMessage(QString("Source set to " + src));
}

void WebPageBuilder::resetRotation()
{
    rotation = 0;
}

void WebPageBuilder::rotateClockwise()
{
    rotation += 90;
}

void WebPageBuilder::rotateCounterCw()
{
    rotation -= 90;
}

void WebPageBuilder::setBgColor(const int &color)
{
    Q_ASSERT(color <= 16777215);
    Q_ASSERT(color >= 0);
    QString colorCode = QString::number(color, 16);
    int fill = 6 - colorCode.length();
    QString fillStr = "";
    while (fill) {
        fillStr += "0";
        fill--;
    }
    bgColor = "#" + fillStr + colorCode;
}

QString WebPageBuilder::buildPage() const
{
    QString logmsg = "Rotation at " + QString::number(rotation) + " degrees";
    logger(logmsg);
    // lost my imagination, sorry. Will fix later.
    QString derp = rotation % 180 == 0 ? "hw" : "wh";
    QString msg = baseStart + bgColor + baseMid1.arg(derp.at(0), derp.at(1)) + QString::number(rotation) + baseMid2 + src + baseEnd;
    logmsg = QUrl::toPercentEncoding(msg);
    logger(logmsg);
    return msg;
}
