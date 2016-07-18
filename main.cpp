#include "mainwindow.h"
#include "viewerwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    ViewerWindow w;
    //Win1 e(&w);
    w.show();
    // this is sort of not good, but whatever.
    MainWindow *mainWin = w.getMainWindow();
    mainWin->show();
    //mainWin->initStuff();
    //mainWin->updateImage();
    //e.show();

    return a.exec();
}
