//
//  TigerNativeFunction.h
//  TigerAPI
//
//  Created by Wang zhen on 4/7/16.
//
//

#ifndef TigerNativeFunction_h
#define TigerNativeFunction_h

#include "native/java/TigerJNICalls.h"
#include "native/objectc/TigerObjectCCalls.h"

namespace Tiger
{
    inline const void imageSaveToPhotoAlbum(const std::string file)
    {
        if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
            TigerObjectCCalls::trySaveImageToPhotoAlbum(file.c_str());
        }
        else if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        {
            TigerJNICalls::trySaveImageToPhotoAlbum(file.c_str());
        }
    }
    
    inline void openWeb(const std::string url)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        TigerJNICalls::tryCallWeb(url.c_str());
#endif
        
    }
    
    inline void openUrl(const std::string url)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        TLog("openUrl -- %s", url.c_str());
        TigerObjectCCalls::tryOpenUrl(url.c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        TigerJNICalls::tryOpenUrl(url.c_str());
#endif
    }
    
    /**
     sendSceneNameToGoogleAnalytics
     @params scene name
     @params version name - ios: fix APP_VERSION_NAME in BaseAppConfig class
                          - android: fix APP_VERSION_NAME in JinHelper.java 
     */
    inline void sendSceneNameToGoogleAnalytics(const std::string scene)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        TigerObjectCCalls::trySendSceneNameToGoogleAnalytics(scene.c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        TigerJNICalls::trySendSceneNameToGoogleAnalytics(scene.c_str());
#endif
    }
    
    inline void callContactUs()
    {
        std::string title = "Aircraft Factory : Please write any inquiry or bug report!";
        std::string content = "Please write any inquiry or bug report! \n";
        
        if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
            TigerObjectCCalls::trySendAnMail("support@yfactory.com", title.c_str(), content.c_str());
        }else if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        {
            TigerJNICalls::trySendAnEmail("support@yfactory.com", title.c_str(), content.c_str());
        }
    }
};

#endif /* TigerNativeFunction_h */




