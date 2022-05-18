#这是一个mac下通过qt调用oc接口的例子，也就是.mm文件
#包含如何让系统不休眠，以及休眠检测，
#申请microphone麦克风权限
#added by zuoshul 20220328
QT       += core gui
TARGET = MacosQtDemo
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    osxsleep.cpp

#调用oc接口，只能写在.mm文件中，不然识别不了oc语法，支持oc和c++语法
OBJECTIVE_SOURCES +=microphone.mm
OBJECTIVE_HEADERS += microphone.h

HEADERS += \
    mainwindow.h \
    osxsleep.h

FORMS += \
    mainwindow.ui

#调用oc库，必须链接相应的oc库，否则编译不过，而且链接格式必须是-framework，而不是-L
QMAKE_LFLAGS += -framework AVFoundation

#oc下必须有一个info.plist，这个文件用来表示一个app的权限描述，签名，证书boudid等
#而且要注意，app编译成功后，不要再去app里面手动修改这个文件，否则会导致很多系统功能失效，比如麦克风权限申请
QMAKE_INFO_PLIST = $$PWD/Info.plist
#QMAKE_POST_LINK += copy $$PWD/Info.plist $${TARGET}.app/Contents/Info.plist

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
