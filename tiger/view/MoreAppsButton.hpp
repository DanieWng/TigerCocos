//
//  MoreAppsButton.hpp
//  TigerAPI
//
//  Created by Wang zhen on 4/5/16.
//
//

#ifndef MoreAppsButton_hpp
#define MoreAppsButton_hpp

#include "cocos2d.h"
#include <ui/CocosGUI.h>

using namespace cocos2d::ui;
USING_NS_CC;

class MoreAppsButton : public Layout
{
public:
    
    enum class MoreAppsButtonType
    {
        kRightTop,
        kRightBottom,
        kLeftTop
    };
    
public:
    
    MoreAppsButton();
    ~MoreAppsButton();
    
    virtual void onEnter() override;
    
    bool initWithType(MoreAppsButtonType type);
    static MoreAppsButton* create(MoreAppsButtonType type);
    
private:
    
    bool initWithRightTop(bool isLastApp);
    bool initWithRightBottom(bool isLastApp);
    bool initWithLeftTop(bool isLastApp);
    
    void playEffectAnimationStart();
    
private:
    
    MoreAppsButtonType _type;
    
    Sprite *_moreText;
    Sprite *_icon;
    
    ClippingNode *_clippingNode;
    Sprite *_diagonalLine;
    
    Vec2 _diagonalStartPos;
    Vec2 _iconTargetPos;
    Vec2 _iconStartPos;
    
    bool _moveToFont;
};

#endif /* MoreAppsButton_hpp */


















