#include "microphone.h"
#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVFCapture.h>
#import <AVFoundation/AVCaptureDevice.h>

//函数声明为c++语法，方便外部调用
//函数实现语法为oc语法
//这种混合语法，只能写在.mm格式文件里面，否则编译不过

int isValidToVisitMicroPhone()
{
    int a = "ddd";
    if(!@available(macOS 10.14, *))
    {
        //macos10.14后才需要申请麦克风权限
        return true;
    }

    __block int nCanRecord = -2;

    //查询该app是否授权麦克风，是未授权，还是拒绝，允许
    AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio];
    NSLog(@"status : %d",status);

    switch (status) {
        case AVAuthorizationStatusNotDetermined:{
            //未授权状态
            //app首次运行时，会进入该状态，请求用户授权
            //不管用户是允许，还是拒绝，后面运行该app再也不会进入此case，除非更换app的boundID，或者重置麦克风授权
           //测试阶段可以使用tccutil reset Microphone命令重置系统麦克风授权
            // 许可对话没有出现，发起授权许可
            //还有一点要注意的是，第一次请求授权，弹出授权框后，本函数就返回了，不会组塞，所以第一次授权本函数返回的是nCanRecord的默认值-2
            [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL granted) {
                if (granted) {
                    //第一次用户接受
                    nCanRecord = 1; //其实无意义，函数已经返回了nCanRecord默认值-2
                    NSLog(@"-- granted");

                }else{
                    //用户拒绝
                    nCanRecord = -1; //其实无意义，函数已经返回了nCanRecord默认值-2
                    NSLog(@"-- not granted");
                }
            }];
            break;
        }

        case AVAuthorizationStatusAuthorized:{
            // 已经开启授权
            NSLog(@"-- Authorized");
            nCanRecord = 0;
            break;
        }
        case AVAuthorizationStatusDenied:{
            // 授权未开启
            NSLog(@"-- Denied");
            nCanRecord = -1;
            break;
        }
        case AVAuthorizationStatusRestricted:{
            NSLog(@"-- Restricted");
            nCanRecord = -3;
            break;
        }

        default:
            break;
    }
    return nCanRecord;
}
