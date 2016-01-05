//
//  TScrollViewBar.hpp
//  TheMillStone
//
//  Created by Wang zhen on 10/19/15.
//
//

#ifndef TScrollViewBar_hpp
#define TScrollViewBar_hpp

#include "TScrollView.h"

namespace Tiger {

class TScrollViewBar : public cocos2d::ProtectedNode
{
public:
    
    TScrollViewBar(TScrollView* parent, TScrollView::Direction direction);
    
    virtual ~TScrollViewBar();
    
    static TScrollViewBar* create(TScrollView *parent, TScrollView::Direction direction);
    
    void setPositionFromCorner(const Vec2& positionFromCorner);
    
    Vec2 getPositionFromCorner() const;

    void setWidth(float width);
    float getWidth() const;
    
    void setAutoHideEnable(bool autoHideEnable) { _autoHideEnable = autoHideEnable; };
    
    bool isAutoHideEnable() const { return _autoHideEnable; }
    
    void setAutoHideTime(float autoHideTime) { _autoHideTime = autoHideTime; }
    float getAutoHideTime() const { return _autoHideTime; }
    
    virtual void onScrolled(const Vec2& outOfBoundary);
    
    virtual void setOpacity(GLubyte opactity) override { _opacity = opactity; }
    virtual GLubyte getOpacity() const override { return _opacity; }
    virtual void onEnter() override;
    virtual void update(float deltaTime) override;
    
    void onTouchBegan();
    
    void onTouchEnded();
    
CC_CONSTRUCTOR_ACCESS:
    virtual bool init() override;
    
protected:
    
    bool _autoHideEnable;
    float _autoHideTime;
    float _autoHideRemainingTime;
    
    GLubyte _opacity;
    
    TScrollView *_parent;
    TScrollView::Direction _direction;
    
    Sprite *_upperHalfCircle;
    Sprite *_lowerHalfCircle;
    Sprite *_body;
    
    float _marginFromBoundary;
    float _marginForLength;
    
    bool _touching;
    
    
private:

    float calculateLength(float innerContainerMeasure, float scrollViewMeasure, float outOfBoundaryValue);
    
    Vec2 calculatePosition(float innnerContainerMeasure, float scrollViewMeasure, float innerContainerPosition, float outOfBounaryValue, float actualLength);
    
    void updateLength(float length);
    void processAutoHide(float deltaTime);
    
    
    
};

}

#endif /* TScrollViewBar_hpp */


















