#include "mainwindow.h"
#include <QApplication>

/********************************
 * Clipboard++
 * Version: 0.0.1
 * Author: Pigoil
 * License: MIT license
********************************/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setQuitOnLastWindowClosed(true);
    w.show();

    return a.exec();
}
