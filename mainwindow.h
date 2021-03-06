#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QUrl>
#include <QVector>

//#include "viewerwindow.h"
//#include "webpagebuilder.h"
#include "config.h"
#include "websocketserver.h"
#include "testview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, testview *view = 0);
    ~MainWindow();

    void initStuff();
    void setSizes();
public slots:
    void TypeURL(const QString &str);
    void LogMessage(const QString &msg, const int &priority = 0);

    void updateImage(bool resetRotation = true);
    void show();

    void changeWebSocketButton(const bool &isOnline);
    void showStatus(const QString &msg);
    void changeImage(const QString &url);
    //void dropEventBridge(QDropEvent *event);
protected:
    virtual void closeEvent(QCloseEvent *);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

private slots:
    void on_actionExit_triggered();

    void on_turnLeftButton_clicked();

    void on_turnRightButton_clicked();

    void wpSizeChanged();

    void saveSettings();
    void on_wsServerButton_clicked();

private:
    Ui::MainWindow *ui;
    //ViewerWindow *viewer;
    testview *viewer;
    //zWebPageBuilder *wpb;
    Config *conf;
    WebsocketServer *server;

    void loadSettings();
};

#endif // MAINWINDOW_H
