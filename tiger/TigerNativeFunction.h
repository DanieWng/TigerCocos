//
//  TigerNativeFunction.h
//  TigerAPI
//
//  Created by Wang zhen on 4/7/16.
//
//

#ifndef TigerNativeFunction_h
#define TigerNativeFunction_h

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "native/java/TigerJNICalls.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <TigerObjectCCalls.h>
#endif

namespace Tiger
{
    inline void imageSaveToPhotoAlbum(const std::string file)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        TigerObjectCCalls::trySaveImageToPhotoAlbum(file.c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        
#endif
    }
    
    inline void openUrl(const std::string url)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        TigerObjectCCalls::tryOpenUrl(url.c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        
#endif
    }
    
};

#endif /* TigerNativeFunction_h */
