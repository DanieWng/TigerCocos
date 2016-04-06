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

#define DEBUG_SHOW 1
#define APP_RELEASE_DATE 1459746912

USING_NS_CC;

class BaseAppConfig
{
public:
    
    BaseAppConfig();
    virtual ~BaseAppConfig();
    
    static BaseAppConfig* getInstance();
    static void destoryInstance();
    
    /*
     multi display functions.
     */
    const Size getDesginResoucesSizeByDevice();
    const float getSceneScaleFactor();
    void setSupportMultiDisplay();
    
    virtual const std::string getLanguageLetter();
    virtual void saveUserDefault();
    
private:
    
    void initUserDefault();
    
protected:
    
    CC_SYNTHESIZE(Tiger::DeviceType, _curDevice, DeviceType);
    CC_SYNTHESIZE(cocos2d::LanguageType, _curLanguage, CurLanguage);
    
    CC_SYNTHESIZE(Tiger::VersionData, _curVersionData, CurVersionData);
    
    CC_SYNTHESIZE_READONLY(std::string, _moreappsPath, MoreAppsPath);
    
    CC_SYNTHESIZE(unsigned int, _lastConnectDate, LastConnectServerData);
    
private:
    
    static BaseAppConfig* _instance;
    
};

#endif /* BaseAppConfig_hpp */












