#include "mainwindow.h"
//#include "viewerwindow.h"
#include "testview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;

    testview t;
    MainWindow w(0, &t);
    t.setConfigWindow(&w);
    t.show();
    w.show();

    //ViewerWindow w;
    //Win1 e(&w);
    //w.show();
    // this is sort of not good, but whatever.
    //MainWindow *mainWin = w.getMainWindow();
    //mainWin->show();
    //mainWin->initStuff();
    //mainWin->updateImage();
    //e.show();

    return a.exec();
}
