//
//  TSlipSwitchButton.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 1/12/16.
//
//

#ifndef TSlipSwitchButton_hpp
#define TSlipSwitchButton_hpp

#include "cocos2d.h"
#include "TigerMacros.h"
#include "TigerEnum.h"

namespace Tiger{

class TSlipSwitchButton : public cocos2d::Node
{
public:
    
    TSlipSwitchButton();
    virtual ~TSlipSwitchButton();
    
    virtual bool init() override;
    
    CREATE_FUNC(TSlipSwitchButton);
    
    void addBar(const std::string bgOnRes, const std::string bgOffRes, bool isPlist=false);
    
    void addButton(const std::string& btnRes, bool isPlist=false);
    
    void setBallAndBarLeftPadding(float v);
    void setBallAndBarRightPadding(float v);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void setSwitchBtnType(Tiger::SwitchButtonType type);
    
    /*
     define delegate
     */
    typedef std::function<void(Ref*, Tiger::SwitchButtonType)> fSwitchButtonTypeChangeDelegate;
    void setSwitchButtonTypeChangeDelegate(const fSwitchButtonTypeChangeDelegate d);
    
protected:
    
    CC_SYNTHESIZE_READONLY(Tiger::SwitchButtonType, _curSwitchBtnType, SwitchBtnType);
    
private:
    
    cocos2d::Vec2 getBallPositionWithSwitchButtonType(Tiger::SwitchButtonType type);
    
    float getPercentWithBallPosition(cocos2d::Vec2 ballPos);
    cocos2d::Vec2 getBallPositionWithPercent();
    
    void setTouchListenerEnable(bool enable);
    
    void setBallPosition(const cocos2d::Vec2 pos, bool isAnimation=false);
    
    void initMaxTouchMoveLength();
    
private:
    
    float _ballLeftPadding;
    float _ballRightPadding;
    
    cocos2d::Sprite *_ballNode;
    cocos2d::Sprite *_barOn;
    cocos2d::Sprite *_barOff;
    
    float _percent;
    float _unitMoveLength;
    float _maxTouchMoveLength;
    
    bool _isTouchMoved;
    
    cocos2d::Vec2 _totalDelta;
    
    fSwitchButtonTypeChangeDelegate _btnTypeChangeDelegate;
};

}

#endif /* TSlipSwitchButton_hpp */






