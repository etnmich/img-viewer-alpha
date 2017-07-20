#include "testview.h"
#include "mainwindow.h"
#include <QBuffer>
#include <QtNetwork>
#include <QDebug>
#include <QImageReader>
#include <QDropEvent>

testview::testview(QWidget *parent, uint32_t width, uint32_t height, uint32_t color) :
    QWidget(parent), currentImage(nullptr), currentMovie(nullptr), rotationAngle(0)
{
    // create view
    layout = new QGridLayout(this);
    this->setLayout(layout);
    scrol = new QScrollArea(this);
    lab = new QLabel(this);

    layout->addWidget(scrol);
    layout->setMargin(0);
    scrol->setWidget(lab);
    scrol->setAlignment(Qt::AlignCenter);
    scrol->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrol->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setAcceptDrops(true);
    scrol->setAcceptDrops(false);
    lab->setAcceptDrops(false);

    setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    resize(width, height);
    setBackgroundColor(color);
    //lab->setStyleSheet("background-color: black");
    //lab->setScaledContents(true);
    //lab->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    //init downloader
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleResponse(QNetworkReply*)));
    setWindowTitle("Image Viewer Viewport");
    //changeUrl("http://i.telegraph.co.uk/multimedia/archive/03589/Wellcome_Image_Awa_3589699k.jpg");
}

testview::~testview()
{
    delete currentImage;
    delete currentMovie;
}

void testview::setConfigWindow(MainWindow *win)
{
    configWindow = win;
}

void testview::setBackgroundColor(const uint32_t &color)
{
    scrol->setStyleSheet(QString("background-color: #%1;").arg(color, 6, 16, QChar('0')));
}

void testview::changeUrl(const QString &url)
{
    QUrl uri = QUrl::fromUserInput(url);
    if (uri.isValid()) {
        manager->get(QNetworkRequest(uri));
        configWindow->TypeURL(uri.toString());
    }

}

void testview::resetRotation()
{
    rotationAngle = 0;
    resizeImage();
}

void testview::rotateLeft()
{
    rotationAngle += 90;
    resizeImage();
}

void testview::rotateRight()
{
    rotationAngle -= 90;
    resizeImage();
}

void testview::dropEventBridge(QDropEvent *event)
{
    dropEvent(event);
}

void testview::loadImage(const QUrl &url)
{
    qDebug() << "loading image";
    //manager = new QNetworkAccessManager(this);
    //connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(stuffHappened(QNetworkReply*)));
    QNetworkRequest req(url);
    QNetworkReply *rep = manager->get(req);
    rep->open(QIODevice::ReadOnly);
    qDebug() << rep->rawHeaderList();
    //QBuffer buf;
    //buf.open(QIODevice::ReadWrite);
    //rep->open(QIODevice::ReadOnly);
    //if (!rep->error()) {
    //    qDebug() << "we have reply";
        //while (!rep->isFinished()) {
            //QByt
            //qDebug() << "wrote" << buf.write(rep->readAll()) << "bytes";
            //rep->readAll()
        //} // get that cpu usage up
    //    buf.write(rep->readAll());
    //}
    //qDebug() << "read" << buf.size() << "bytes";
}

QSize testview::getScaledSize(const QSize &originalSize)
{
    qDebug() << "orig" << originalSize;
    uint32_t currentWidth = originalSize.width();
    uint32_t currentHeight = originalSize.height();
    double ratio = (double)currentWidth / (double)currentHeight;
    double vpRatio = (double)scrol->width() / (double)scrol->height();
    QSize r;
    if (vpRatio > ratio) {
        auto newHeight = scrol->height();
        r = QSize(ratio*newHeight, newHeight);
    } else {
        auto newWidth = scrol->width();
        r = QSize(newWidth, newWidth/ratio);
    }
    qDebug() << "new" << r;
    return r;
}

void testview::resizeEvent(QResizeEvent *event)
{
    //qDebug() << "resized";
    QWidget::resizeEvent(event);
    configWindow->setSizes();
    resizeImage();
}

void testview::dropEvent(QDropEvent *event)
{
   const QMimeData *mimez = event->mimeData();
   if (mimez->hasImage()) {
       qDebug() << "it haz the imgs, size is" << mimez->imageData().toSize();
       // possibly do something
   }
   if (mimez->hasUrls()) {
       qDebug() << "it haz the urls";
       if (mimez->urls().length() > 1)
           qDebug() << "so many urls," << mimez->urls();
       else {
           qDebug() << "it iz" << mimez->urls()[0];
       }
       manager->get(QNetworkRequest(mimez->urls()[0]));
       resetRotation();
   }
}

void testview::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void testview::closeEvent(QCloseEvent *)
{
    configWindow->close();
}
/*
void testview::stuffHappened(QNetworkReply *rep)
{
    qDebug() << "finished";
    if (auto err = rep->error()) {
        qDebug() << err;
        return;
    }
    if (rep->open(QIODevice::ReadOnly)) {
        //QBuffer buf;
        //buf.open(QIODevice::ReadWrite);
        //qDebug() << "reply is" << rep->size();
        //buf.write(rep->readAll());
        //qDebug() << "buffer is" << buf.size();
        changeImage(new QImage(QImageReader(rep).read()));
        resizeImage();
        //QImage img = QImageReader(rep).read();//(&buf).read();
        //qDebug() << img.height() << img.width();
        //img = img.scaled(scrol->size(), Qt::KeepAspectRatio);
        //lab->resize(img.size());
        //lab->setPixmap(QPixmap::fromImage(img));
    }
}
*/
void testview::resizeImage()
{
    if (currentImage) {
        QTransform trans;
        trans.rotate(rotationAngle);
        QImage img = currentImage->transformed(trans).scaled(scrol->size(), Qt::KeepAspectRatio);
        //img = img.transformed(trans);
        lab->resize(img.size());
        lab->setPixmap(QPixmap::fromImage(img));
    } else if (currentMovie)
    {
        QSize size = getScaledSize(movieSize);
        currentMovie->stop();
        currentMovie->setScaledSize(size);
        lab->resize(size);
        currentMovie->start();
    }
/*
    if (!currentMovie)
        return;
    currentMovie->setScaledSize(scrol->size());
    lab->resize(currentMovie->scaledSize());
    lab->setMovie(currentMovie);
    currentMovie->start();*/
}

void testview::changeImage(/*QMovie *newMovie)*/QImage *newImage)
{
    removeMovie();
    removeImage();
    currentImage = newImage;/*
    if (currentMovie)
        delete currentMovie;
    currentMovie = newMovie;*/
}

void testview::changeMovie(QMovie *newMovie)
{
    removeMovie();
    removeImage();
    // loop
    //newMovie
    currentMovie = newMovie;
    connect(currentMovie, SIGNAL(finished()), currentMovie, SLOT(start()));
    lab->setMovie(currentMovie);
}

void testview::handleResponse(QNetworkReply *rep)
{
    if (auto err = rep->error()) {
        qDebug() << err;
        return;
    }

    qDebug() << "Headers:";
    foreach (auto header, rep->rawHeaderList()) {
        qDebug() << header << "-" << rep->rawHeader(header);
    }
    QString mimeType = "null/null";
    if (!rep->url().isLocalFile()) {
        QString mimeType = qvariant_cast<QString>(rep->header(QNetworkRequest::ContentTypeHeader));
        if (mimeType.split('/')[0] != "image")
            qDebug() << "not image but a" << mimeType;
    }
    // TODO: error handling
    if (mimeType.split('/')[1] == "gif" || rep->url().toString().right(3) == "gif") {
        qDebug() << "it's a gif";
        auto mov = new QMovie(this);
        mov->setCacheMode(QMovie::CacheAll);
        mov->setDevice(rep);
        mov->jumpToFrame(0);
        movieSize = mov->currentImage().size();
        changeMovie(mov);
    } else {
        qDebug() << "just a pic";
        changeImage(new QImage(QImageReader(rep).read()));
    }
    resizeImage();
}

void testview::removeImage()
{
    if (currentImage)
        delete currentImage;
    currentImage = nullptr;
}

void testview::removeMovie()
{
    if (currentMovie) {
        lab->setMovie(0);
        disconnect(currentMovie, SIGNAL(finished()), currentMovie, SLOT(start()));
        currentMovie->deleteLater();
    }
    currentMovie = nullptr;
}

