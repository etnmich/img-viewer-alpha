#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO: switch this to be an option
const int DEBUG_PRIORITY = 0;



MainWindow::MainWindow(QWidget *parent, ViewerWindow *view) :
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
    ui->LogBrowser->append(logMsg);
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
    wpb->setSource(QUrl(ui->URLline->text()));
    wpb->setBgColor(ui->bgColor->value());
    if (resetRotation)
    {
        wpb->resetRotation();
    }
    viewer->renderPage(wpb->buildPage());
}

void MainWindow::show()
{
    // short redef, shit breaks up if updated before show
    QWidget::show();
    setSizes();
    updateImage();

    connect(ui->updateButton, SIGNAL(clicked(bool)), this, SLOT(updateImage()));
    connect(ui->wpHeight, SIGNAL(valueChanged(int)), this, SLOT(wpSizeChanged()));
    connect(ui->wpWidth, SIGNAL(valueChanged(int)), this, SLOT(wpSizeChanged()));
    connect(ui->saveButton, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));

    connect(ui->actionShow_Log, SIGNAL(toggled(bool)), ui->LogLabel, SLOT(setVisible(bool)));
    connect(ui->actionShow_Log, SIGNAL(toggled(bool)), ui->LogBrowser, SLOT(setVisible(bool)));
    ui->actionShow_Log->toggle();
    resize(400, 100);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent * event)
{

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

      ui->URLline->setText(firstUrl);
      updateImage();
    }
}

void MainWindow::on_turnLeftButton_clicked()
{
    wpb->rotateCounterCw();
    updateImage(false);
}

void MainWindow::on_turnRightButton_clicked()
{
    wpb->rotateClockwise();
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
        QString col = conf->getKey("bgColor");
        ui->bgColor->setValue(col.toInt());
    } catch (...) {}
    try {
        QString h = conf->getKey("wpHeight");
        ui->wpHeight->setValue(h.toInt());
    } catch (...) {}
    try {
        QString w = conf->getKey("wpWidth");
        ui->wpWidth->setValue(w.toInt());
    } catch (...) {}
}

void MainWindow::saveSettings()
{
    conf->setKey("bgColor", QString::number(ui->bgColor->value()));
    conf->setKey("wpHeight", QString::number(ui->wpHeight->value()));
    conf->setKey("wpWidth", QString::number(ui->wpWidth->value()));
    conf->save();
}

