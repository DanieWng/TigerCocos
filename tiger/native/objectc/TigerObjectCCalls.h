//
//  TigerObjectCCalls.h
//  TheSolarSystem
//
//  Created by Wang zhen on 10/27/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#ifndef __TheSolarSystem__TigerObjectCCalls__
#define __TheSolarSystem__TigerObjectCCalls__

#include <stddef.h>


class TigerObjectCCalls
{
    
public:
    
    static void trySendAnMail(const char* address, const char* title, const char* body);
    
    static void trySendAnMailWithImage(const char* address, const char* title, const char* body, const char* imageName);
    
    static bool tryIsInternetcConnection();
    
    static void tryCallWeb(const char* url);
    
    static void trySaveImageToPhotoAlbum(const char* fileName);
    
};

#endif /* defined(__TheSolarSystem__TigerObjectCCalls__) */
