#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();  //如果没有a.exec()将会导致休眠检测失效，因为休眠检测依赖事件循环，没有怎么办呢，用CFRunLoopRun();代替
}
