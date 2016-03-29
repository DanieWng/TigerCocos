//
//  AppConfig.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 1/13/16.
//
//

#include "AppConfig.hpp"
#include "PlaneInfoData.hpp"
#include "PlanePropertyData.hpp"

#define DEFAULT_SCENE_SIZE          Size(1024, 768)
#define DEFAULT_SCENE_SCALE_FACTOR  1.0f

#define KEY_USERDEFAULT_IS_EXIST    "userdefault_is_exist"
#define KEY_CUR_LANGUAGE            "cur_language"
#define KEY_BADGE_CATEGORY1 "badge_category1"
#define KEY_BADGE_CATEGORY2 "badge_category2"
#define KEY_BADGE_CATEGORY3 "badge_category3"
#define KEY_BADGE_CATEGORY4 "badge_category4"
#define KEY_BADGE_CATEGORY5 "badge_category5"
#define KEY_BADGE_CATEGORY6 "badge_category6"

USING_NS_CC;

AppConfig* AppConfig::_instance = nullptr;

AppConfig::AppConfig()
{
    setDeviceType(Tiger::DeviceType::kiPad);
    setCurLanguage(cocos2d::LanguageType::ENGLISH);
    
    setMusicEnable(true);
    setSFXEnable(true);
    
    setCurPlaneID(0);
    
    _curGotBadge.clear();
    _curGotBadge[BadgeCategory::kCategory1] = {BADGE_CATEGORY1_TOTAL};
    _curGotBadge[BadgeCategory::kCategory2] = {BADGE_CATEGORY2_TOTAL};
    _curGotBadge[BadgeCategory::kCategory3] = {BADGE_CATEGORY3_TOTAL};
    _curGotBadge[BadgeCategory::kCategory4] = {BADGE_CATEGORY4_TOTAL};
    _curGotBadge[BadgeCategory::kCategory5] = {BADGE_CATEGORY5_TOTAL};
    _curGotBadge[BadgeCategory::kCategory6] = {BADGE_CATEGORY6_TOTAL};
    
    initUserDefault();
}

AppConfig::~AppConfig()
{
    // 销毁单例类
    PlanePropertyModel::destoryInstance();
    
    /*
     销毁'wirteable path/usr/0/'文件夹
     */
}

AppConfig* AppConfig::getInstance()
{
    if (!_instance)
    {
        _instance = new (std::nothrow)AppConfig();
    }
    
    return _instance;
}

void AppConfig::destoryInstance()
{
    if (_instance)
    {
        CC_SAFE_DELETE(_instance);
        _instance = nullptr;
    }
}

cocos2d::Size AppConfig::getDesginResoucesSizeByDevice()
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

float AppConfig::getSceneScaleFactor()
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

const std::string AppConfig::getLanguageLetter()
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

void AppConfig::initUserDefault()
{
    auto ud = UserDefault::getInstance();
    
    if (!ud->isXMLFileExist())
    {
        ud->setIntegerForKey(KEY_BADGE_CATEGORY1, BADGE_CATEGORY1_TOTAL);
        ud->setIntegerForKey(KEY_BADGE_CATEGORY2, BADGE_CATEGORY2_TOTAL);
        ud->setIntegerForKey(KEY_BADGE_CATEGORY3, BADGE_CATEGORY3_TOTAL);
        ud->setIntegerForKey(KEY_BADGE_CATEGORY4, BADGE_CATEGORY4_TOTAL);
        ud->setIntegerForKey(KEY_BADGE_CATEGORY5, BADGE_CATEGORY5_TOTAL);
        ud->setIntegerForKey(KEY_BADGE_CATEGORY6, BADGE_CATEGORY6_TOTAL);
        
        ud->flush();
    }
    else
    {
        addGotBadge(BadgeCategory::kCategory1, ud->getIntegerForKey(KEY_BADGE_CATEGORY1));
        addGotBadge(BadgeCategory::kCategory2, ud->getIntegerForKey(KEY_BADGE_CATEGORY2));
        addGotBadge(BadgeCategory::kCategory3, ud->getIntegerForKey(KEY_BADGE_CATEGORY3));
        addGotBadge(BadgeCategory::kCategory4, ud->getIntegerForKey(KEY_BADGE_CATEGORY4));
        addGotBadge(BadgeCategory::kCategory5, ud->getIntegerForKey(KEY_BADGE_CATEGORY5));
        addGotBadge(BadgeCategory::kCategory6, ud->getIntegerForKey(KEY_BADGE_CATEGORY6));
    }
}

void AppConfig::saveUserDefault()
{
    auto ud = UserDefault::getInstance();
    
    ud->setIntegerForKey(KEY_BADGE_CATEGORY1, _curGotBadge.at(BadgeCategory::kCategory1));
    ud->setIntegerForKey(KEY_BADGE_CATEGORY2, _curGotBadge.at(BadgeCategory::kCategory2));
    ud->setIntegerForKey(KEY_BADGE_CATEGORY3, _curGotBadge.at(BadgeCategory::kCategory3));
    ud->setIntegerForKey(KEY_BADGE_CATEGORY4, _curGotBadge.at(BadgeCategory::kCategory4));
    ud->setIntegerForKey(KEY_BADGE_CATEGORY5, _curGotBadge.at(BadgeCategory::kCategory5));
    ud->setIntegerForKey(KEY_BADGE_CATEGORY6, _curGotBadge.at(BadgeCategory::kCategory6));
    
    ud->flush();
}

void AppConfig::addGotBadge(BadgeCategory category, int index)
{
    switch (category)
    {
        case kCategory1:
            index = MIN(index, BADGE_CATEGORY1_TOTAL);
            break;
            
        case kCategory2:
            index = MIN(index, BADGE_CATEGORY2_TOTAL);
            break;
            
        case kCategory3:
            index = MIN(index, BADGE_CATEGORY3_TOTAL);
            break;
            
        case kCategory4:
            index = MIN(index, BADGE_CATEGORY4_TOTAL);
            break;
            
        case kCategory5:
            index = MIN(index, BADGE_CATEGORY5_TOTAL);
            break;
            
        case kCategory6:
            index = MIN(index, BADGE_CATEGORY6_TOTAL);
            break;
    }
    index = MAX(index, 0);

    _curGotBadge.at(category) = {index};
}

void AppConfig::setAppConfig()
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









