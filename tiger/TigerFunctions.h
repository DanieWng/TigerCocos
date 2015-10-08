//
//  TigerFunctions.h
//  Yoohoo
//
//  Created by Wang zhen on 5/13/15.
//
//

#ifndef Yoohoo_TigerFunctions_h
#define Yoohoo_TigerFunctions_h

#include "cocos2d.h"
#include "TCustomToast.h"
#include <ui/CocosGUI.h>
#include "TigerEnum.h"

using namespace cocos2d::ui;
USING_NS_CC;

namespace T
{
    inline void setPosAndAnchorPointAndTag(Ref* target, Vec2 pos, Vec2 anchor, int tag)
    {
        auto t = (Node*)target;
        t->setPosition(pos);
        t->setAnchorPoint( anchor );
        t->setTag(tag);
    }
    
    inline void setTalkNodePosAndAnchorPoint(Sprite* talk, Vec2 pos, float startYInNodeSpace, float anchorX = 0)
    {
        talk->setPosition( Vec2(pos.x, pos.y - startYInNodeSpace) );
        
        float anchor_y;
        
        anchor_y = startYInNodeSpace / talk->getContentSize().height;
        
        talk->setAnchorPoint( Vec2(anchorX, 1 - anchor_y) );
    }
    
    inline void setTalkNodePosAndAnchorPointTopBottomMode(Node* talk, Vec2 pos, float startXInNodeSpace, float anchorY = 0)
    {
        talk->setPosition( Vec2(pos.x + startXInNodeSpace, pos.y) );
        
        float anchor_x = startXInNodeSpace / talk->getContentSize().width;
        
        talk->setAnchorPoint( Vec2(anchor_x, anchorY) );
    }
    
    inline ActionInterval* callTalkShowAction()
    {
        auto action = Spawn::create(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)),
                                    FadeTo::create(0.5f, 255),
                                    nullptr);
        
        return action;
    }
    
    inline ActionInterval* callTalkHideAction()
    {
        auto action = Spawn::create(EaseBackIn::create(ScaleTo::create(0.5f, 0.1f)),
                                    FadeTo::create(0.5f, 0),
                                    nullptr);
        
        return action;
    }
    
    inline void removeUnusedTextureCache()
    {
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }
    
    inline void removeUnusedSpriteFrame()
    {
        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    }
    
    inline void destoreAnimationCache()
    {
        AnimationCache::getInstance()->destroyInstance();
    }
    
    inline void removeAllSpriteFrame()
    {
        SpriteFrameCache::getInstance()->removeSpriteFrames();
    }
    
    /**
     SpriteFrameCache, TextureCache, FileCache...
     */
    inline void purgeDataCache()
    {
        Director::getInstance()->purgeCachedData();
    }
    
    inline void clearMemory()
    {
        AnimationCache::destroyInstance();
        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }
    
    inline void setMenuSelectImageColor(MenuItemSprite* item)
    {
        item->getSelectedImage()->setColor(cocos2d::Color3B(122, 122, 122));
    }
    
    inline const std::string getCurrentDate()
    {
        struct tm* tm;
        time_t timep;
        
        timeval tv;
        gettimeofday(&tv, nullptr);
        timep = tv.tv_sec;
        
        tm = localtime(&timep);
        int year = tm->tm_year + 1900;
//        int month = tm->tm_mon + 1;
//        int day = tm->tm_mday;
        
        return __String::createWithFormat("%d", year)->getCString();
    }
    
    inline void showToast(const std::string& msg, Node* parent)
    {
        auto toast = CustomToast::create(msg);
        parent->addChild(toast, 100);
        toast->show();
    }
    
//    inline std::string getFullPath(const std::string& path)
//    {
//        
//        std::string p="";
//        
//        if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID &&
//            app::AppConfig::getInstance()->getAndroidMarket() == ANDROID_MARKET::GOOGLE)
//        {
//            p.append(FileUtils::getInstance()->getWritablePath()).append(path);
//        }else
//        {
//            p.append(path);
//        }
//
//        return p;
//
//    }
    
    inline void setButtonIsEnable(Button *btn, bool isEnable)
    {
        if (isEnable)
        {
            btn->setBright(true);
            btn->setEnabled(true);
            btn->setTouchEnabled(true);
        }else
        {
            btn->setBright(false);
            btn->setEnabled(false);
            btn->setTouchEnabled(false);
        }
    }
    
    
    
}



#endif
