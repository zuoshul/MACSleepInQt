#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <IOKit/pwr_mgt/IOPMLib.h>
#include <QTimer>
#include "osxsleep.h"
#include "microphone.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer::singleShot(1000,this,SLOT(stopSleep()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stopSleep()
{
    //禁止系统闲置休眠，也就是不动电脑的情况，但是对手动强制休眠不生效
    g_OSXSleepPro->instance()->disableSystemSleep();

    sleep(5);

    //取消禁止系统闲置休眠
   g_OSXSleepPro->instance()->enableSystemSleep();

   g_OSXSleepPro->instance()->regNotify();

   //int status = isValidToVisitMicroPhone();

   //qDebug()<<"status="<<status;
}


