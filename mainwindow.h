#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QUrl>

#include "viewerwindow.h"
#include "webpagebuilder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, ViewerWindow *view = 0);
    ~MainWindow();

    void initStuff();
    void setSizes();
public slots:
    void TypeURL(const QString &str);
    void LogMessage(const QString &msg, const int &priority = 0);

    void updateImage(bool resetRotation = true);
    void show();

protected:
    virtual void closeEvent(QCloseEvent *);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

private slots:
    void on_actionExit_triggered();

    void on_turnLeftButton_clicked();

    void on_turnRightButton_clicked();

    void wpSizeChanged();

private:
    Ui::MainWindow *ui;
    ViewerWindow *viewer;
    WebPageBuilder *wpb;
};

#endif // MAINWINDOW_H
