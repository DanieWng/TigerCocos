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
    kiPhpne4s,       // 960x640
    kiPhone5,        // 1136x640
    kiphone6,        // 1334x750
    kiphone6plus,    // 1920x1080
    kiPad,           // 1024x768
    kiPad_Retina,    // 2048x1536
    kAndroid_HD,
    kAndroid_SD
};

enum class ANDROID_MARKET
{
    kGOOGLE,
    kOther
};

enum class InternetAllow
{
    kDeny,
    kAllow
};

enum class SwitchButtonType
{
    kOn,
    kOff
};

NS_T_END

#endif
