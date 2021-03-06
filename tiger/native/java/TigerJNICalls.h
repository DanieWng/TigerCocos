//
//  TigerJNICalls.h
//  TheSolarSystem
//
//  Created by Wang zhen on 10/28/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#ifndef __TheSolarSystem__TigerJNICalls__
#define __TheSolarSystem__TigerJNICalls__

#include <stdio.h>

class TigerJNICalls
{
public:
    
    static void trySendAnEmail(const char* addres, const char* title, const char* body);
    
    static void trySendAnEmailWithImage(const char* address, const char* title, const char* body, const char* imagefile);
    
    static void tryCallWeb(const char* urlStr);
    
    static bool tryIsInternetcConnection();
    
    static const char* tryGetDeviceLanguage();
    
    static long  tryGetVailableMemorySize();
    
    static void trySaveImageToPhotoAlbum(const char* fileName);
    static const char* trySaveImageToSdcard(const char* fileName);
  
    static void tryOpenUrl(const char* url);
    
    static void trySendSceneNameToGoogleAnalytics(const char* scene);
};


#endif /* defined(__TheSolarSystem__TigerJNICalls__) */
