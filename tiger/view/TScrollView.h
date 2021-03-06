//
//  TigerScrollView.h
//  Yoohoo
//
//  Created by Wang zhen on 5/14/15.
//
//

#ifndef __Yoohoo__TigerScrollView__
#define __Yoohoo__TigerScrollView__

#include "cocos2d.h"
#include <ui/CocosGUI.h>

USING_NS_CC;
using namespace ui;

namespace Tiger
{

class TScrollViewBar;
    
class TScrollView : public Layer
{
public:
    
    enum Direction
    {
        VERTICAL,
        HORIZONTAL,
        BOTH
    };
    
    enum class EventType
    {
        SCROLL_TO_TOP,
        SCROLL_TO_BOTTOM,
        SCROLL_TO_LEFT,
        SCROLL_TO_RIGHT,
        SCROLLING,
        SCROLLING_DONE,
        BOUNCE_TOP,
        BOUNCE_BOTTOM,
        BOUNCE_LEFT,
        BOUNCE_RIGHT,
        BOUNCE_DONE,
        SLIDING,
        SLID_ACTION_DONE,
        CLICK_CELL,
        ADJUST_DONE,
        TOUCH_CANCELED
    };
    
    // Define scrolling direction.
    const Vec2 SCROLLDIR_UP     = Vec2(0.0f, 1.0f);
    const Vec2 SCROLLDIR_DOWN   = Vec2(0.0f, -1.0f);
    const Vec2 SCROLLDIR_LEFT   = Vec2(-1.0f, 0.0f);
    const Vec2 SCROLLDIR_RIGHT  = Vec2(1.0f, 0.0f);
    
    typedef std::function<void(Ref*, EventType)> tScrollViewCallback;
    
    /**
     Defualt constructor and destructor
     */
    TScrollView();
    virtual ~TScrollView();
    
    /**
     Allocates and initializes.
     */
    static TScrollView* create(const Size& size);
    static TScrollView* create(const Size& size, const Color4B& color);
    
    bool initWithSize(const Size& size);
    bool initWithSizeAndColor4B(const Size& size, const Color4B& color);
    
    virtual void onEnter() override;
    
    /**
     Get chrildrens of container.
     */
    Layout* getContainer() { return _container; };
    
    /**
     Set container size of scroll view.
     */
    void setContainerSize(const Size& size);
    Size getContainerSize() { return _containerSize; };
    
    // add cell to container.
    void addItem(Node* item);
    
    // About Scroll View Offset method.
    void setContentOffset(Vec2 offset, bool isAnim=false, float timer=1.0f);
    Vec2 getContentOffset() { return _offset; };
    
    void moveToTop();
    void moveToBottom();
    void moveToLeft();
    void moveToRight();
    
    void moveToNext();
    void moveToPre();
    void moveToCurPageOriginPos();
    
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onTouchMoved(Touch* touch, Event* event) override;
    virtual void onTouchEnded(Touch* touch, Event* event) override;
    
    void setTouchListenerSwallow(bool isSwallow);
    void setTouchListenerEnable(bool enable);
    void setTouchEnable(bool enable);
    
    /**
     Add call back function called scroll view event triggered.
     */
    virtual void addEventListener(const tScrollViewCallback& callback);
    
    virtual void update(float dt) override;
    
    void setClippingToBounds(bool bClippingToBounds) { _clippingToBounds = bClippingToBounds; }
    
    void setScrollViewBarEnable(bool enable);
    
    void setScrollBarWidth(float width);
    float getScrollBarWidth() const;
    
    void setScrollBarColor(const Color3B& color);
    const Color3B& getScrollBarColor() const;
    
    void setScrollBarOpacity(GLubyte opacity);
    GLubyte getScrollBarOpacity() const;
    
    void setScrollBarAutoHideEnable(bool autoHideEnable);
    bool isScrollBarAutoHideEnable() const;
    
    void setScrollBarAutoHideTime(float autoHideTime);
    float getScrollBarAutoHideTime() const;
    
    void setScrollBarPositionFromCorner(const Vec2& positionFromCorner);
    
    void initScrollBar();
    
protected:
    
    CC_SYNTHESIZE(Direction, _direction,        Direction);
    
    // container margin value.
    CC_SYNTHESIZE(float,     _topBottomMargin,  TopBottomMargin);
    CC_SYNTHESIZE(float,     _leftRightMargin,  LeftRightMargin);
    
    CC_SYNTHESIZE_READONLY(int, _clickedTag, ClickCellTag);
    
    CC_SYNTHESIZE(int , _curPageIndex, CurrentPageIndex);
    
    CC_SYNTHESIZE_READONLY(Rect, _frameRect, FrameRect);
    
    CC_SYNTHESIZE_READONLY(bool, _isMoved, IsMoved);
    
    CC_SYNTHESIZE(float, _scrollSpeedFactor, AutoScrollSpeedFactor);
    
    CC_SYNTHESIZE(float, _vecticalMinEffectiveDistance, VecticalMinEffectiveDistance);
    
    CC_SYNTHESIZE(float, _horizontalMinEffectDistance, HorizontalMinEffectiveDistance);
    
    Layout* _container;
    Size   _containerSize;
    
    Vec2   _offset;
    
    EventListenerTouchOneByOne *_touchListener;
    
    Vec2   _touchBeganInPosition;
    Vec2   _touchMovedInPosition;
    Vec2   _touchEndedInPosition;
    
    // About bounce.
    bool   _bouncing;
    CC_SYNTHESIZE(bool, _bounceEnable, BounceEnable);
    
    // Is touched began.
    bool   _isPressed;
    
    Vec2   _lastDelta;
    float  _slidTime;
    Vec2   _autoScrollDir;
    float  _autoScrollOriginalSpeed;
    bool   _autoScroll;
    
    bool   _topBounceNeeded;
    bool   _bottomBounceNeeded;
    bool   _leftBounceNeeded;
    bool   _rightBounceNeeded;
    float  _bounceOriginalSpeed;
    
    tScrollViewCallback _eventCallback;
    
    bool _isTouchEnable;
    
protected:
    
    void   recordSlidTime(float dt); // 记录拖动时间
    void   startRecordSlidAction();  // 初始化拖动时间
    void   endRecordSlidAction();    // 拖动结束，自动滑动方向和速度
    
    void   stopAutoScrollChildren(); // 自动滑动动作结束，初始化相关变量
    void   stopBounceChildren();     //
    
    // 获取新的滑动方向和速度
    void   startAutoScrollChildrenWithOriginalSpeed(const Vec2& dir, float v);
    
    /**
      当自动滑动布尔变量_autoScroll是true时，
      每帧按照速度递减的公式，重新设置scroll view的偏移量
     
       speed *= AUTO_SCROLL_SPEED_FACTOR
       offset.y += (speed * dir.y)
       setContentOffset(offset)
     
     */
    void   autoScrollChildren();
    
    // 计算在滑动或拖动scroll view时，是否有超过允许的边界
    bool   checkOffsetIfOutrideBounceBoundary();
    bool   checkOffsetIfOutrideBoundary();
    
    // 回调函数
    void   scrollTo(EventType event);
    
    // 检测是否反弹
    bool   checkIsBounce();
    
    void   startBounceBack(); // 设置反弹开始
    void   endBounceBack();   // 设置反弹结束
    void   bounceChildren();  // 反弹函数
    
    void   computePageIndex();
    
private:
    
    CustomCommand _beforeDrawCommand;
    CustomCommand _afterDrawCommand;
    
    bool _clippingToBounds;
    
    /**
     * scissor rect for parent, just for restoring GL_SCISSOR_BOX
     */
    Rect _parentScissorRect;
    bool _scissorRestored;
    
    bool _scrollBarEnabled;
    TScrollViewBar* _verticalScrollBar;
    
private:
    
    /**
     * clip this view so that outside of the visible bounds can be hidden.
     */
    void beforeDraw();
    void onBeforeDraw();
    /**
     * retract what's done in beforeDraw so that there's no side effect to
     * other nodes.
     */
    void afterDraw();
    void onAfterDraw();

    /**
     * @js NA
     * @lua NA
     */
    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    
    /**
     关于scroll view 滚动条的函数
     */
    void updateScrollBar(const Vec2& outOfBoundary);
    void setScrollBarPositionFromCornerForVertical(const Vec2& positionFromCorner);
    Vec2 getScrollBarPositionFromCornerForVertical() const;
    
    Vec2 getHowMuchOutOfBoundary(const Vec2& addition) const;
};
    
    
}



#endif /* defined(__Yoohoo__TigerScrollView__) */
