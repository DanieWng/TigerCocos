//
//  AppConfig.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 1/13/16.
//
//

#ifndef AppConfig_hpp
#define AppConfig_hpp

#include "cocos2d.h"
#include "tiger/TigerEnum.h"
#include "GameData.h"
#include "PlaneProperty.h"

#define DEBUG_SHOW 1

class AppConfig
{
public:
    
    AppConfig();
    ~AppConfig();
    
    static AppConfig* getInstance();
    static void destoryInstance();
    
    cocos2d::Size getDesginResoucesSizeByDevice();
    float getSceneScaleFactor();
    
    const std::string getLanguageLetter();
    
    void saveUserDefault();
    
    std::unordered_map<int, int>& getCurGotBadge() { return _curGotBadge; }
    
    void addGotBadge(BadgeCategory category, int index);
    
    void setAppConfig();
    
private:
    
    void initUserDefault();
    
protected:
    
    CC_SYNTHESIZE(Tiger::DeviceType, _curDevice, DeviceType);
    CC_SYNTHESIZE(cocos2d::LanguageType, _curLanguage, CurLanguage);
    
    /*
     var 
     about setting
     */
    CC_SYNTHESIZE(bool, _musicEnable, MusicEnable);
    CC_SYNTHESIZE(bool, _sfxEnable, SFXEnable);
    
    CC_SYNTHESIZE(unsigned int, _curPlaneID, CurPlaneID);
    
private:
    
    static AppConfig* _instance;
    
    std::unordered_map<int, int> _curGotBadge;
    
};

#endif /* AppConfig_hpp */














