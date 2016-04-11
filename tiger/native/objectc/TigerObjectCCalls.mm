//
//  TigerObjectCCalls.cpp
//  TheSolarSystem
//
//  Created by Wang zhen on 10/27/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#include "TigerObjectCCalls.h"
#import "MailSender.h"
#import "WebView.h"
#import "Reachability.h"
#import <SystemConfiguration/SystemConfiguration.h>
#import <AVFoundation/AVFoundation.h>
#import "cocos2d.h"
//#import "CCEAGLView.h"
#import "AppController.h"

void TigerObjectCCalls::trySendAnMail(const char *address, const char *title, const char *body)
{
    MailSender *mailSender = [MailSender alloc];

    [ mailSender sendMailUsingInAppMailer:address title:title body:body ];

}

void TigerObjectCCalls::trySendAnMailWithImage(const char *address, const char *title, const char *body, const char *imageName)
{
    MailSender *mailSender = [MailSender alloc];
    
    [ mailSender sendMailUsingInAppMailer:address title:title body:body image:imageName ];
}

void TigerObjectCCalls::tryCallWeb(const char *url)
{
    WebView *webView = [WebView alloc];
    
    [webView setLayerWebView:url];

}

bool TigerObjectCCalls::tryIsInternetcConnection()
{
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    NetworkStatus nextworkStatus = [reachability currentReachabilityStatus];
    
    switch (nextworkStatus) {
        case NotReachable:
            return false;
            break;
        
        case ReachableViaWiFi:
            return true;
            break;
        
        case ReachableViaWWAN:
            return true;
            break;
    }
}

void TigerObjectCCalls::trySaveImageToPhotoAlbum(const char *fileName)
{
    NSString *path = [NSString stringWithUTF8String:fileName];
    
    UIImage *image = [UIImage imageWithContentsOfFile:path];
    
    UIImageWriteToSavedPhotosAlbum(image,
                                   nil,
                                   nil,
                                   nil);
}

void TigerObjectCCalls::tryOpenUrl(const char *url)
{
    NSURL *nsurl = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
    [[UIApplication sharedApplication] openURL:nsurl];
}








