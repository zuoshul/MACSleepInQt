#include "osxsleep.h"
#include <IOKit/pwr_mgt/IOPMLib.h>
#include <IOKit/IOMessage.h>
#include <QDebug>

//我们这个文件，调用的是macOS系统API，但是采用的是c++语法，可能这个类库比较好支持c++语法方式调用？
//如果不好用c++语法调用，建议用oc语法调用，因为这类源码网上较多，比较方便，但是文件格式就只能用.mm而不能用.cpp

io_connect_t  root_port;

OSXSleep::OSXSleep() : m_assertionID(-1)
{

}

OSXSleep::~OSXSleep()
{
    enableSystemSleep();
}

void OSXSleep::disableSystemSleep()
{
    if(m_assertionID >= 0)
    {
        return;
    }
    CFStringRef reasonForActivity= CFSTR("Describe Activity Type");
    IOPMAssertionID assertionID;
    IOReturn success = IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep,kIOPMAssertionLevelOn, reasonForActivity, &assertionID);
    if (success == kIOReturnSuccess)
    {
        m_assertionID = assertionID;
    }
}

void OSXSleep::enableSystemSleep()
{
    if(m_assertionID >= 0)
    {
        IOReturn success = IOPMAssertionRelease(m_assertionID);
        if (success == kIOReturnSuccess)
        {
            m_assertionID = -1;
        }
    }
}

void MySleepCallBack( void * refCon, io_service_t service, natural_t messageType, void * messageArgument )
{
    switch (messageType)
    {
        case kIOMessageSystemWillSleep:
            qCritical()<<"kIOMessageSystemWillSleep";   //即将进入休眠
            g_OSXSleepPro->instance()->isSleeping(true); //发出即将进入休眠信号，其他接受类做相应处理
            IOAllowPowerChange(root_port, (long)messageArgument); //处理完了，允许继续休眠
            break;
        case kIOMessageSystemHasPoweredOn:
            qCritical()<<"kIOMessageSystemHasPoweredOn"; //即将从休眠中唤醒
            g_OSXSleepPro->instance()->isSleeping(false); //发出即将从休眠中唤醒信号，其他接受类做相应处理
            break;
        default:
            break;
    }
}

void OSXSleep::runLoop()
{
    //该接口是为程序添加事件循环，没有事件循环将无法检测到休眠
    //由于qt本身具备事件循环，即main函数里面到a.exec();所以qt程序无需调用本接口
    //但是非qt程序，比如控制台程序，自身不具备事件循环，就需要在main函数里面调用该接口，增加事件循环
    CFRunLoopRun();
}

void OSXSleep::regNotify()
{
    IONotificationPortRef  notifyPortRef;
    io_object_t            notifierObject;
    void*                  refCon;
    root_port = IORegisterForSystemPower( refCon, &notifyPortRef, MySleepCallBack, &notifierObject );  //注册休眠通知回调
    if ( root_port != 0 )
    {
        //进入事件循环，不然无法进回调检测休眠，CFRunLoopGetCurrent()是获取当前程序的事件循环
        CFRunLoopAddSource(CFRunLoopGetCurrent(),IONotificationPortGetRunLoopSource(notifyPortRef), kCFRunLoopCommonModes);
    }
}

