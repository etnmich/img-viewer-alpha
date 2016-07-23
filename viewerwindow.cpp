#include "viewerwindow.h"
#include "ui_viewerwindow.h"
#include "mainwindow.h"

ViewerWindow::ViewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow)
{
    ui->setupUi(this);

    //Qt::WindowFlags flags = windowFlags();
    //setWindowFlags(Qt::WindowStaysOnBottomHint);
    //show();
    setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    win = new MainWindow(0, this);
    //ui->webView->load("");//(QUrl("http://www.google.com"));
}

ViewerWindow::~ViewerWindow()
{
    delete ui;
}

void ViewerWindow::SwitchURL(const QString url)
{
    win->LogMessage("Switching URL to " + url);
    ui->webView->setUrl(url);
}

void ViewerWindow::renderPage(const QString &pageData)
{
    win->LogMessage("re-rendering page");
    //ui->webView->setHtml(pageData);
    if (page) { page->deleteLater(); }
    page = new QWebEnginePage(this);
    page->setHtml(pageData);
    ui->webView->setPage(page);
    //ui->webView->setHtml();
}

void ViewerWindow::closeEvent(QCloseEvent *)
{
    win->close();
}

MainWindow *ViewerWindow::getMainWindow()
{
    return win;
}

void ViewerWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);
    win->setSizes();
}

void ViewerWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void ViewerWindow::dropEvent(QDropEvent *event)
{
    qDebug() << event;
    win->dropEventBridge(event);
}
