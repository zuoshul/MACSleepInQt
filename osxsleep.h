#ifndef OSXSLEEP_H
#define OSXSLEEP_H

//#include <IOKit/pwr_mgt/IOPMLib.h>          头文件放在这里和c++混编有时候报错，放在.cpp好一点
#include <QObject>

//added by zuoshul 20220323

class OSXSleep : public QObject
{
    Q_OBJECT
public:
    static OSXSleep* instance()
    {
        static OSXSleep* _osxSleep = NULL;
        if(NULL == _osxSleep)
        {
            _osxSleep = new OSXSleep();
        }
        return _osxSleep;
    }

    ~OSXSleep();

    void disableSystemSleep();

    void enableSystemSleep();

    void regNotify();

    void runLoop();

signals:
    bool isSleeping(bool sleep);

private:
    OSXSleep();

private:
    int m_assertionID;
};

#define g_OSXSleepPro OSXSleep::instance()

#endif // OSXSLEEP_H
