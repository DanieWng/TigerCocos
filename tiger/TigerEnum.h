//
//  TigerEnum.h
//  MusicTreePart1
//
//  Created by Wang zhen on 8/10/15.
//
//

#ifndef MusicTreePart1_TigerEnum_h
#define MusicTreePart1_TigerEnum_h

#include "TigerMacros.h"

NS_T_BEGIN

enum DeviceType
{
    iPhpne4s,       // 960x640
    iPhone5,        // 1136x640
    iphone6,        // 1334x750
    iphone6plus,    // 1920x1080
    iPad,           // 1024x768
    iPad_Retina,    // 2048x1536
    Android_HD,
    Android_SD
};

enum ANDROID_MARKET
{
    GOOGLE,
    Other
};

enum PageTag
{
    Tag_NoPage = 100,
    Tag_Page1,
    Tag_Page2,
    Tag_Page3,
    Tag_Page4,
    Tag_Page5,
    Tag_Page6,
    Tag_Page7,
    Tag_Page8,
    Tag_Page9,
    Tag_Main,
    Tag_MyPage,
    Tag_Info,
    
};

NS_T_END

#endif
