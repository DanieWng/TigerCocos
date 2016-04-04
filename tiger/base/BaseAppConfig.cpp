//
//  BaseAppConfig.cpp
//  TigerAPI
//
//  Created by Wang zhen on 4/4/16.
//
//

#include "BaseAppConfig.hpp"

#define DEFAULT_SCENE_SIZE          Size(1024, 768)
#define DEFAULT_SCENE_SCALE_FACTOR  1.0f

#define KEY_USERDEFAULT_IS_EXIST    "userdefault_is_exist"
#define KEY_CUR_LANGUAGE            "cur_language"
#define KEY_DOWNLOADED_MOREAPPS     "is_downloaded_moreapps"


BaseAppConfig* BaseAppConfig::_instance = nullptr;

BaseAppConfig::BaseAppConfig()
{
    setDeviceType(Tiger::DeviceType::kiPad);
    setCurLanguage(cocos2d::LanguageType::ENGLISH);
    
    _curVersionData = Tiger::VersionData();
    
    setIsDownloadedMoreApps(false);
    
    initUserDefault();
}

BaseAppConfig::~BaseAppConfig()
{
}

BaseAppConfig* BaseAppConfig::getInstance()
{
    if (!_instance)
    {
        _instance = new (std::nothrow)BaseAppConfig();
    }
    
    return _instance;
}

void BaseAppConfig::destoryInstance()
{
    if (_instance)
    {
        CC_SAFE_DELETE(_instance);
        _instance = nullptr;
    }
}

const Size BaseAppConfig::getDesginResoucesSizeByDevice()
{
    Size ret = DEFAULT_SCENE_SIZE;
    
    switch (_curDevice)
    {
        case Tiger::DeviceType::kiPad_Retina:
        case Tiger::DeviceType::kiphone6plus:
            return ret*2;
            break;
            
        default:
            break;
    }
    
    return ret;
}

const float BaseAppConfig::getSceneScaleFactor()
{
    float ret = DEFAULT_SCENE_SCALE_FACTOR;
    
    switch (_curDevice)
    {
        case Tiger::DeviceType::kiPad_Retina:
        case Tiger::DeviceType::kiphone6plus:
            return ret*2.0f;
            break;
            
        default:
            break;
    }
    
    return ret;
}

const std::string BaseAppConfig::getLanguageLetter()
{
    std::string ret = "e";
    
    switch (_curLanguage)
    {
        case cocos2d::LanguageType::KOREAN:
            
            break;
            
        default:
            break;
    }
    
    return ret;
}

void BaseAppConfig::initUserDefault()
{
    auto ud = UserDefault::getInstance();
    
    if (!ud->getBoolForKey(KEY_USERDEFAULT_IS_EXIST, false))
    {
        // init user default
        ud->setBoolForKey(KEY_USERDEFAULT_IS_EXIST, true);
        ud->setIntegerForKey(KEY_CUR_LANGUAGE, static_cast<int>(_curLanguage));
        
        ud->setIntegerForKey(VERSION_JSON_MEMBER_VERSION, _curVersionData._version);
        ud->setIntegerForKey(VERSION_JSON_MEMBER_LAST_RELEASE_DATE, _curVersionData._lastAppReleaseDate);
        ud->setStringForKey(VERSION_JSON_MEMBER_NEW_DOWNLOAD_URL, _curVersionData._resDownloadUrl);
        
        ud->setBoolForKey(KEY_DOWNLOADED_MOREAPPS, _isDownloadedMoreApps);
        
        ud->flush();
    }
    else
    {
        // get data from user default
        _curLanguage = static_cast<LanguageType>(ud->getIntegerForKey(KEY_CUR_LANGUAGE));
        
        _curVersionData._version = ud->getIntegerForKey(VERSION_JSON_MEMBER_VERSION);
        _curVersionData._lastAppReleaseDate = ud->getIntegerForKey(VERSION_JSON_MEMBER_LAST_RELEASE_DATE);
        _curVersionData._resDownloadUrl = ud->getStringForKey(VERSION_JSON_MEMBER_NEW_DOWNLOAD_URL);
    
        _isDownloadedMoreApps = ud->getBoolForKey(KEY_DOWNLOADED_MOREAPPS);
    }
}

void BaseAppConfig::saveUserDefault()
{
    auto ud = UserDefault::getInstance();
    
    ud->setIntegerForKey(KEY_CUR_LANGUAGE, static_cast<int>(_curLanguage));
    
    ud->setIntegerForKey(VERSION_JSON_MEMBER_VERSION, _curVersionData._version);
    ud->setIntegerForKey(VERSION_JSON_MEMBER_LAST_RELEASE_DATE, _curVersionData._lastAppReleaseDate);
    ud->setStringForKey(VERSION_JSON_MEMBER_NEW_DOWNLOAD_URL, _curVersionData._resDownloadUrl);
    
    ud->setBoolForKey(KEY_DOWNLOADED_MOREAPPS, _isDownloadedMoreApps);
    
    UserDefault::getInstance()->flush();
}

/*
 set support multi display
 */
void BaseAppConfig::setSupportMultiDisplay()
{
    Size win_size = Director::getInstance()->getWinSize();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if ((int)win_size.width % 1024 == 0 || (int)win_size.height % 1024 == 0)
    {
        // this is iPad
        if (win_size.width > 1024 || win_size.height > 1024)
        {
            // air, air2, mini retina
            setDeviceType(Tiger::DeviceType::kiPad_Retina);
            FileUtils::getInstance()->addSearchPath("image/2048/");
            
        }else
        {
            // ipad2, mini
            setDeviceType(Tiger::DeviceType::kiPad);
            FileUtils::getInstance()->addSearchPath("image/1024/");
        }
        
    }else if ((int)win_size.width == 960)
    {
        setDeviceType(Tiger::DeviceType::kiPhpne4s);
        FileUtils::getInstance()->addSearchPath("image/1024/");
        
    }else if((int)win_size.width == 1136)
    {
        setDeviceType(Tiger::DeviceType::kiPhone5);
        FileUtils::getInstance()->addSearchPath("image/1024/");
        
    }else if ((int)win_size.width == 1334)
    {
        setDeviceType(Tiger::DeviceType::kiphone6);
        FileUtils::getInstance()->addSearchPath("image/1024/");
        
    }else if ((int)win_size.width >= 1920)
    {
        setDeviceType(Tiger::DeviceType::kiphone6plus);
        FileUtils::getInstance()->addSearchPath("image/2048/");
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if ((int)win_size.width % 1024 == 0 || (int)win_size.height % 1024 == 0)
    {
        // 4 : 3, as Mi Pad.
        setDeviceType(Tiger::DeviceType::kiPad);
        FileUtils::getInstance()->addSearchPath("image/1024/");
        
    }else if((int)win_size.width >= 1920)
    {
        // 16 : 9, as 1920 x 1080
        setDeviceType(Tiger::DeviceType::kAndroid_HD);
        FileUtils::getInstance()->addSearchPath("image/1024/");
        
    }else
    {
        // as 1280 x 720
        setDeviceType(Tiger::DeviceType::kAndroid_SD);
        FileUtils::getInstance()->addSearchPath("image/1024/");
    }
#endif
    
    Size desgin_size = getDesginResoucesSizeByDevice();
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(desgin_size.width,
                                                                      desgin_size.height,
                                                                      ResolutionPolicy::FIXED_HEIGHT);
}














