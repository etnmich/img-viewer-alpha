#ifndef TESTVIEW_H
#define TESTVIEW_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QUrl>
#include <QtNetwork>
#include <QScrollArea>
#include <QMovie>
#include <QTransform>

class MainWindow;

class testview : public QWidget
{
    Q_OBJECT
public:
    explicit testview(QWidget *parent = 0, uint32_t width = 800, uint32_t height = 600, uint32_t color = 0);
    ~testview();

    void setConfigWindow(MainWindow *win);

signals:

public slots:
    void setBackgroundColor(const uint32_t &color);
    void changeUrl(const QString &url);

    void resetRotation();
    void rotateLeft();
    void rotateRight();
    void dropEventBridge(QDropEvent *event);

private:
    QLabel *lab;
    QGridLayout *layout;
    QNetworkAccessManager *manager;
    QScrollArea *scrol;
    QImage *currentImage;
    QMovie *currentMovie;
    QSize movieSize;
    qreal rotationAngle;
    MainWindow * configWindow;

    void loadImage(const QUrl &url);
    QSize getScaledSize(const QSize &originalSize);

protected:
    void resizeEvent(QResizeEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void closeEvent(QCloseEvent *event);


private slots:
    //void stuffHappened(QNetworkReply *rep);

    void resizeImage();
    void changeImage(QImage *newImage);
    void changeMovie(QMovie *newMovie);
    void handleResponse(QNetworkReply *rep);

    void removeImage();
    void removeMovie();

};

#endif // TESTVIEW_H
