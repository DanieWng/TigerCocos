//
//  BaseAppConfig.hpp
//  TigerAPI
//
//  Created by Wang zhen on 4/4/16.
//
//

#ifndef BaseAppConfig_hpp
#define BaseAppConfig_hpp

#include "cocos2d.h"
#include "TigerEnum.h"
#include "TigerMoreAppsVersionUtil.hpp"

#define DEBUG_SHOW          0
#define APP_RELEASE_DATE    1465401600
#define APP_VERSION_NAME    "release 1.0"

USING_NS_CC;

class BaseAppConfig
{
public:
    
    BaseAppConfig();
    virtual ~BaseAppConfig();
    
    static BaseAppConfig* getInstance();
    virtual void destoryInstance();
    
    /*
     multi display functions.
     */
    const float getSceneScaleFactor();
    void setSupportMultiDisplay();
    
    virtual const std::string getLanguageLetter();
    virtual void saveUserDefault();
    
protected:
    
    /**
     初始化 user default 配置文件
     @return bool - If true, then means not exist file and created one.
     If false, then means has exist file and get data from file.
     */
    virtual bool initUserDefault();
    
    cocos2d::Size getDesginResoucesSizeByDevice();
    
protected:
    
    CC_SYNTHESIZE(Tiger::DeviceType, _curDevice, DeviceType);
    CC_SYNTHESIZE(cocos2d::LanguageType, _curLanguage, CurLanguage);
    
    CC_SYNTHESIZE(Tiger::VersionData, _curVersionData, CurVersionData);
    
    CC_SYNTHESIZE_READONLY(std::string, _moreappsPath, MoreAppsPath);
    
    CC_SYNTHESIZE(unsigned int, _lastConnectDate, LastConnectServerData);
    
protected:
    
    static BaseAppConfig* _instance;
     
};

#endif /* BaseAppConfig_hpp */












