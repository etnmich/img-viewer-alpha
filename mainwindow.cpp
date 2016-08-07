#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO: switch this to be an option
const int DEBUG_PRIORITY = 0;



MainWindow::MainWindow(QWidget *parent, testview *view) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    viewer = view;

    wpb = new WebPageBuilder(this);
    setAcceptDrops(true);

    conf = new Config("settings.conf", this);
    // do something if error.
    loadSettings();
    //updateImage();

    //viewer->show();
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    server = new WebsocketServer("Image Receiver", false, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
   close();
}

void MainWindow::TypeURL(const QString &str)
{
    ui->URLline->setText(str);
}

void MainWindow::LogMessage(const QString &msg, const int &priority)
{
    if (priority < DEBUG_PRIORITY)
    {
        //skip extraneous log messages
        return;
    }
    QString timestampStr, logMsg;
    timestampStr = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
    logMsg = timestampStr + msg;
    qDebug() << logMsg;
    //QString prevText = ui->textEdit->text
}

void MainWindow::closeEvent(QCloseEvent *)
{
    viewer->close();
    //QMainWindow::closeEvent(evt);
}

void MainWindow::updateImage(bool resetRotation)
{
    LogMessage("Updated image");
    /*
    wpb->setSource(QUrl(ui->URLline->text()));
    wpb->setBgColor(ui->bgColor->value());*/
    viewer->setBackgroundColor(ui->bgColor->value());
    if (resetRotation)
    {
        viewer->resetRotation();
    }
    viewer->changeUrl(ui->URLline->text());
    wpSizeChanged();
    //viewer->renderPage(wpb->buildPage());
}

void MainWindow::show()
{
    // short redef, shit breaks up if updated before show
    QWidget::show();
    setSizes();


    connect(ui->updateButton, SIGNAL(clicked(bool)), this, SLOT(updateImage()));
    //connect(ui->wpHeight, SIGNAL(valueChanged(int)), this, SLOT(wpSizeChanged()));
    //connect(ui->wpWidth, SIGNAL(valueChanged(int)), this, SLOT(wpSizeChanged()));
    connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    //connect(ui->actionShow_Log, SIGNAL(toggled(bool)), ui->LogLabel, SLOT(setVisible(bool)));
    //connect(ui->actionShow_Log, SIGNAL(toggled(bool)), ui->LogBrowser, SLOT(setVisible(bool)));
    ui->actionShow_Log->toggle();
    resize(400, 100);

    connect(server, SIGNAL(ServerOnline(bool)), this, SLOT(changeWebSocketButton(bool)));
    connect(server, SIGNAL(serverMsg(QString)), this, SLOT(showStatus(QString)));
    connect(server, SIGNAL(ImageUrlReceived(QString)), this, SLOT(changeImage(QString)));

    updateImage();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent * event)
{
    viewer->dropEventBridge(event);
/*
    const QMimeData* mimeData = event->mimeData();

    LogMessage(mimeData->text());
    LogMessage(mimeData->html());

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
      //QStringList pathList;
      QList<QUrl> urlList = mimeData->urls();

      // load the first one, if there were multiple
      // TODO: do something about the rest
      QString firstUrl = urlList.first().toString();

      changeImage(firstUrl);
    }*/
}

void MainWindow::changeImage(const QString &url) {
    ui->URLline->setText(url);
    updateImage();
}

void MainWindow::on_turnLeftButton_clicked()
{
    //wpb->rotateCounterCw();
    viewer->rotateLeft();
    updateImage(false);
}

void MainWindow::on_turnRightButton_clicked()
{
    //wpb->rotateClockwise();
    viewer->rotateRight();
    updateImage(false);
}

void MainWindow::wpSizeChanged()
{
    LogMessage("size changed");
    viewer->resize(ui->wpWidth->value(), ui->wpHeight->value());
}

void MainWindow::setSizes()
{
    ui->wpHeight->setValue(viewer->height());
    ui->wpWidth->setValue(viewer->width());
}

void MainWindow::loadSettings()
{
    // do something about this.
    try {
        ui->bgColor->setValue(conf->getKey("bgColor").toInt());
    } catch (...) {}
    try {
        ui->wpHeight->setValue(conf->getKey("wpHeight").toInt());
    } catch (...) {}
    try {
        ui->wpWidth->setValue(conf->getKey("wpWidth").toInt());
    } catch (...) {}
    wpSizeChanged();
}

void MainWindow::saveSettings()
{
    conf->setKey("bgColor", QVariant(ui->bgColor->value()));
    conf->setKey("wpHeight", QVariant(ui->wpHeight->value()));
    conf->setKey("wpWidth", QVariant(ui->wpWidth->value()));
    conf->save();
}

void MainWindow::changeWebSocketButton(const bool &isOnline)
{
    ui->wsServerButton->setText(QString("%1 WebSocket server").arg(isOnline ? "Stop" : "Start"));

}

void MainWindow::on_wsServerButton_clicked()
{
    if (server->isOnline()) {
        server->stop();
        return;
    }
    //ui->statusBar->showMessage("help");
    server->start(ui->wsPort->value());
    qDebug() << server->isOnline();
}

void MainWindow::showStatus(const QString &msg)
{
    ui->statusBar->showMessage(msg);
}
/*
void MainWindow::dropEventBridge(QDropEvent *event)
{
    dropEvent(event);
}*/
