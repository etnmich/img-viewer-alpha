#ifndef WIN1_H
#define WIN1_H

#include <QMainWindow>
#include <QtWebKit>

class MainWindow;

namespace Ui {
class ViewerWindow;
}

class ViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewerWindow(QWidget *parent = 0);
    ~ViewerWindow();

    MainWindow *getMainWindow();

public slots:
    void SwitchURL(QString url);
    void renderPage(const QString &pageData);

protected:
    void closeEvent(QCloseEvent *);

    void resizeEvent(QResizeEvent *event);
private:
    Ui::ViewerWindow *ui;
    MainWindow *win;
};

#endif // WIN1_H
