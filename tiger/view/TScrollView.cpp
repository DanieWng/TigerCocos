//
//  TigerScrollView.cpp
//  Yoohoo
//
//  Created by Wang zhen on 5/14/15.
//
//

#include "TScrollView.h"
#include "TigerMacros.h"
#include "TigerFunctions.h"
#include "TScrollViewBar.hpp"

using namespace Tiger;

#define MIN_EFFECTIVE_DISTANCE_TO_MOVE          2.0f
#define MIN_EFFECTIVE_DISTANCE_TO_SLID          5
#define AUTO_SCROLL_MAX_SPEED                   500.0f
#define BOUNCE_ACTION_TIMER                     0.3f
#define AUTO_SCROLL_SPEED_FACTOR                0.9f
#define SCROLL_VIEW_BAR_TAG                     8282

TScrollView::TScrollView():
_isPressed(false),
_bouncing(false),
_autoScroll(false),
_bounceEnable(true),
_lastDelta(Vec2::ZERO),
_offset(Vec2::ZERO),
_slidTime(0.0f),
_touchBeganInPosition(Vec2::ZERO),
_touchMovedInPosition(Vec2::ZERO),
_touchEndedInPosition(Vec2::ZERO),
_autoScrollDir(Vec2::ZERO),
_autoScrollOriginalSpeed(0),
_bounceOriginalSpeed(0),// @Unused.
_bottomBounceNeeded(false),
_topBottomMargin(false),
_leftBounceNeeded(false),
_rightBounceNeeded(false),
_curPageIndex(0),
_isMoved(false),
_container(nullptr),
_clippingToBounds(false),
_scissorRestored(false)
{
    setTopBottomMargin(0.0f);
    setLeftRightMargin(0.0f);
    setAutoScrollSpeedFactor(AUTO_SCROLL_SPEED_FACTOR);
    
    _verticalScrollBar = nullptr;
    
    _vecticalMinEffectiveDistance = MIN_EFFECTIVE_DISTANCE_TO_MOVE;
}

TScrollView::~TScrollView()
{
    TLog("TScrollView::~TScrollView()");
    
    setTouchEnable(false);
    _verticalScrollBar = nullptr;
}

TScrollView* TScrollView::create(const cocos2d::Size &size)
{
    auto pRet = new (std::nothrow)TScrollView();
    
    if (pRet && pRet->initWithSize(size))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

TScrollView* TScrollView::create(const cocos2d::Size &size, const cocos2d::Color4B &color)
{
    auto pRet = new (std::nothrow)TScrollView();
    
    if (pRet && pRet->initWithSizeAndColor4B(size, color))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool TScrollView::initWithSize(const cocos2d::Size &size)
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0), size.width, size.height))
    {
        return false;
    }
    
    // 设置可触摸范围，默认位置起点为（0，0）.
    _frameRect = Rect(0, 0, size.width, size.height);
    
    _clippingToBounds = true;
    
    return true;
}

bool TScrollView::initWithSizeAndColor4B(const cocos2d::Size &size, const cocos2d::Color4B &color)
{
    if (!LayerColor::initWithColor(color, size.width, size.height))
    {
        return false;
    }
    
    this->setContentSize(size);
    
    // 设置可触摸范围，默认位置起点为（0，0）.
    _frameRect = Rect(0, 0, size.width, size.height);
    
    return true;
}

void TScrollView::onEnter()
{
    LayerColor::onEnter();
    
    this->scheduleUpdate();
}

void TScrollView::setContainerSize(const cocos2d::Size &size)
{
    if (_container == nullptr)
    {
        _container = LayerColor::create(Color4B(0, 0, 0, 0));
        _container->setAnchorPoint(Vec2::ZERO);
        this->addChild(_container);
    }
    
    _container->setContentSize(size);
    _containerSize = size;
}

void TScrollView::addItem(cocos2d::Node *item)
{
    _container->addChild(item);
}

void TScrollView::setContentOffset(cocos2d::Vec2 offset, bool isAnim)
{
    if (!isAnim)
    {
        _container->setPosition(offset);
    }else
    {
        _container->runAction(MoveTo::create(1.0f, offset));
    }
    
    _offset = offset;
    
    Vec2 outOfBoundary = getHowMuchOutOfBoundary(Vec2::ZERO);
    updateScrollBar(outOfBoundary);
}

void TScrollView::moveToTop()
{
    Vec2 offset = Vec2(0, -_containerSize.height + _frameRect.size.height);
    
    setContentOffset(offset);
}

void TScrollView::moveToBottom()
{
    Vec2 offset = Vec2::ZERO;
    
    setContentOffset(offset);
}

/*--------------------------------------------------*/
/**
 Todo: next version add Horizontal
 */
void TScrollView::moveToLeft()
{
    Vec2 offset = Vec2::ZERO;
    
    setContentOffset(offset);
}

void TScrollView::moveToRight()
{
    Vec2 offset = Vec2(-_containerSize.width + _frameRect.size.width, 0);
    
    setContentOffset(offset);
}
/*--------------------------------------------------*/

void TScrollView::setTouchEnable(bool enabled)
{
    if (!enabled)
    {
        _eventDispatcher->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }else
    {
        _eventDispatcher->removeEventListener(_touchListener);
        _touchListener = nullptr;
        
        _touchListener = EventListenerTouchOneByOne::create();
        
        _touchListener->setSwallowTouches(true);
        
        _touchListener->onTouchBegan = CC_CALLBACK_2(TScrollView::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(TScrollView::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(TScrollView::onTouchEnded, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }
    
}

void TScrollView::recordSlidTime(float dt)
{
    if (_isPressed)
    {
        _slidTime += dt;
    }
}

void TScrollView::update(float dt)
{
    if (_autoScroll)
    {
        autoScrollChildren();
    }
    
    if (_bouncing)
    {
        bounceChildren();
    }
    
    recordSlidTime(dt);
}


void TScrollView::autoScrollChildren()
{
    _autoScrollOriginalSpeed *= _scrollSpeedFactor;
    
    if (_direction == VERTICAL)
    {
        float y = _autoScrollOriginalSpeed * _autoScrollDir.y;
        _offset.y += y;
        
    }else if (_direction == HORIZONTAL)
    {
        float x = _autoScrollOriginalSpeed * _autoScrollDir.x;
        
        _offset.x += x;
    }
    
    if (_bounceEnable)
    {
        checkOffsetIfOutrideBounceBoundary();
    }else
    {
        checkOffsetIfOutrideBoundary();
    }
    
    this->setContentOffset(_offset);
    
    scrollTo(TScrollView::EventType::SLIDING);
    
    if ((int)fabsf(_autoScrollOriginalSpeed) <= 1)
    {
        stopAutoScrollChildren();
        
        scrollTo(TScrollView::EventType::SLID_ACTION_DONE);
        
        checkIsBounce();
    }

}

void TScrollView::bounceChildren()
{
    
    Vec2 move_to = _offset;
    
    switch (_direction)
    {
        case VERTICAL:
            if (_bottomBounceNeeded)
            {
                move_to = Vec2::ZERO;
                
            }else if (_topBounceNeeded)
            {
                move_to = Vec2(0.0f, -_containerSize.height+_frameRect.size.height);
            }
            break;
            
        case HORIZONTAL:
            if (_leftBounceNeeded)
            {
                move_to = Vec2::ZERO;
                
            }else if (_rightBounceNeeded)
            {
                move_to = Vec2(-_containerSize.width+_frameRect.size.width, 0);
            }
            break;
    }
    
    _container->runAction(Sequence::create(EaseBackOut::create(MoveTo::create(BOUNCE_ACTION_TIMER, move_to)),
                                           CallFunc::create(LAMBDA_FUNCTION_START
                                                            this->scrollTo(TScrollView::EventType::BOUNCE_DONE);
                                                            LAMBDA_FUNCTION_END),
                                           nullptr));

    
    endBounceBack();
}

void TScrollView::startRecordSlidAction()
{
    if (_autoScroll)
    {
        stopAutoScrollChildren();
    }
    
    _slidTime = 0.0f;
}

void TScrollView::stopAutoScrollChildren()
{
    _autoScroll = false;
    _autoScrollOriginalSpeed = 0.0f;
    
    _lastDelta = Vec2::ZERO;
    _autoScrollDir = Vec2::ZERO;
}


/**
 Computing slid direction and speed.
 */
void TScrollView::endRecordSlidAction()
{
    if (_slidTime <= 0.016f)
    {
        return;
    }
    
    float totalDis = 0.0f;
    float speed = 0.0f;
    Vec2 dir;
    Vec2 touchBeganPositionInNodeSpace = this->convertToNodeSpace(_touchBeganInPosition);
    Vec2 touchEndedPositionInNodeSpace = this->convertToNodeSpace(_touchEndedInPosition);
    
    switch (_direction)
    {
        case VERTICAL:
            
            totalDis = touchEndedPositionInNodeSpace.y - touchBeganPositionInNodeSpace.y;
            
            if (totalDis < 0.0f)
            {
                dir = SCROLLDIR_DOWN;
            }else
            {
                dir = SCROLLDIR_UP;
            }
            
            speed = fabsf(_lastDelta.y);
            
            break;
            
        case HORIZONTAL:
            
            totalDis = touchEndedPositionInNodeSpace.x - touchBeganPositionInNodeSpace.x;
            
            if (totalDis < 0.0f)
            {
                dir = SCROLLDIR_LEFT;
            }else
            {
                dir = SCROLLDIR_RIGHT;
            }
            
            speed = fabsf(_lastDelta.x);
            
            break;
    }
    
    startAutoScrollChildrenWithOriginalSpeed(dir, speed);
    
    _slidTime = 0.0f;
}

void TScrollView::startAutoScrollChildrenWithOriginalSpeed(const cocos2d::Vec2 &dir, float v)
{
    _autoScrollDir = dir;
    _autoScrollOriginalSpeed = v;
    
    TLog("auto scroll original speed[%f]", v);
}

bool TScrollView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    _touchBeganInPosition = touch->getLocation();
    auto touchBeganInNodeSpace = this->convertToNodeSpace(_touchBeganInPosition);
    
    if (!_frameRect.containsPoint(touchBeganInNodeSpace)) return false;
    
    startRecordSlidAction();
    _isPressed = true;
    _isMoved = false;
   
    _offset = _container->getPosition();
    
    if (_verticalScrollBar != nullptr)
    {
        _verticalScrollBar->onTouchBegan();
    }
    
    return true;
}

void TScrollView::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    _touchMovedInPosition = touch->getLocation();
    Vec2 touchMovedInNodeSpace = this->convertToNodeSpace(_touchMovedInPosition);
    Vec2 touchPreviousInNodeSpace = this->convertToNodeSpace(touch->getPreviousLocation());
    
    // Get last delta value.
    _lastDelta = touchMovedInNodeSpace - touchPreviousInNodeSpace;
    
    
    if (_direction == Direction::VERTICAL)
    {
        // fabsf(_lastDelta.y) < 2.0f
        if (fabsf(_touchMovedInPosition.y - _touchBeganInPosition.y) < _vecticalMinEffectiveDistance)
        {
            return;
        }else
        {
            if (_offset.y > 0 || fabsf(_offset.y) > (_containerSize.height - _contentSize.height))
            {
                _lastDelta.y *= 0.15f;
            }
            
            _offset.y += _lastDelta.y;
            _isMoved = true;
            
        }
        
    }else if (_direction == Direction::HORIZONTAL)
    {
        if (fabsf(_lastDelta.x) < MIN_EFFECTIVE_DISTANCE_TO_MOVE)
        {
            return;
        }else
        {
            _offset.x += _lastDelta.x;
            
            _isMoved = true;
        }
    }
    
    if (_bounceEnable)
    {
        checkOffsetIfOutrideBounceBoundary();
    }else
    {
        checkOffsetIfOutrideBoundary();
    }
    
    this->setContentOffset(_offset);
    
    scrollTo(TScrollView::EventType::SCROLLING);
    
}

void TScrollView::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    _isPressed = false;

    _touchEndedInPosition = touch->getLocation();
    
    Vec2 delta = _touchEndedInPosition - _touchBeganInPosition;
    
    // Its scroll event.
    switch (_direction)
    {
        case VERTICAL:
            // If bigger than MIN_EFFECTIVE_DISTANCE_TO_SLID, then begin to auto scroll.
            if (fabsf(_lastDelta.y) > MIN_EFFECTIVE_DISTANCE_TO_SLID / 2.0f)
            {
                _autoScroll = true;
                TLog("TScrollView::onTouchEnded --> auto scroll is true [Vertical]");
            }
            break;
            
        case HORIZONTAL:
            if (fabsf(_lastDelta.x) > MIN_EFFECTIVE_DISTANCE_TO_SLID)
            {
                _autoScroll = true;
                TLog("TScrollView::onTouchEnded --> auto scroll is true [Horizontal]");
            }
            break;
    }
    
    endRecordSlidAction();
    
    if (!_autoScroll)
    {
        checkIsBounce();
    }
    
    if (_verticalScrollBar != nullptr)
    {
        _verticalScrollBar->onTouchEnded();
    }
    
    this->scrollTo(TScrollView::EventType::TOUCH_CANCELED);
}

/**
 检测是否越界。
 1. 当越界反弹设置为false时，return false
 2. 当越界时，计算反弹方向和速度, 设置反弹动作为true，将会在下一帧update（）刷新时，执行反弹动作。 return true
 3. 当没有越界时，return false
 */
bool TScrollView::checkIsBounce()
{
    if (!_bounceEnable)
    {
        scrollTo(TScrollView::EventType::SCROLLING_DONE);
        
        computePageIndex();
        
        return false;
    }
    
    switch (_direction)
    {
        case VERTICAL:
            if (_offset.y > 0)
            {
                _bottomBounceNeeded = true;
                
                // @Unused.
                _bounceOriginalSpeed = fabsf(_offset.y - 0.0f);
                
                scrollTo(TScrollView::EventType::BOUNCE_BOTTOM);
                
            }else if (_offset.y < -_containerSize.height + _frameRect.size.height)
            {
                _topBounceNeeded = true;
                
                // @Unused.
                _bounceOriginalSpeed = fabsf(_offset.y - (-_containerSize.height + _frameRect.size.height));
                
                scrollTo(TScrollView::EventType::BOUNCE_TOP);
            }else
            {
                scrollTo(TScrollView::EventType::SCROLLING_DONE);
                return false;
            }
            break;
            
        case HORIZONTAL:
            // TODO: Horizontal mode.
            
            if (_offset.x > 0)
            {
                _leftBounceNeeded = true;
                
                // @Unused.
                 _bounceOriginalSpeed = fabsf(_offset.x - 0.0f);
                
                scrollTo(TScrollView::EventType::BOUNCE_LEFT);
                
            }else if (_offset.x < -_containerSize.width + _frameRect.size.width)
            {
                _rightBounceNeeded = true;
                
                // @Unused.
                _bounceOriginalSpeed = fabsf(_offset.x - (-_containerSize.width + _frameRect.size.width));
                
                scrollTo(TScrollView::EventType::BOUNCE_RIGHT);
                
            }else
            {
                scrollTo(TScrollView::EventType::SCROLLING_DONE);
            }
            break;
    }
    
    startBounceBack();
    
    return true;
}

/**
 计算在滑动或拖动scroll view时，是否有超过允许的边界
 Todo: 实际上并不能对偏移值进行限制工作。
 */
bool TScrollView::checkOffsetIfOutrideBounceBoundary()
{
    
    switch (_direction)
    {
        case VERTICAL:
            if (_offset.y >= _frameRect.size.height - _topBottomMargin)
            {
                _offset.y = _frameRect.size.height - _topBottomMargin;
                return true;
                
            }else if (_offset.y < -_containerSize.height + _topBottomMargin)
            {
                _offset.y = -_containerSize.height + _topBottomMargin;
                return true;
            }
            break;
            
        case HORIZONTAL:
            if (_offset.x >= _frameRect.size.width - _leftRightMargin)
            {
                _offset.x = _frameRect.size.width - _leftRightMargin;
                return true;
                
            }else if (_offset.x < -_containerSize.width + _leftRightMargin)
            {
                _offset.x = -_containerSize.width + _leftRightMargin;
                return true;
            }
            break;
    }
    
    return false;
}

/**
 计算在滑动或拖动scroll view时，是否有超过允许的边界
 */
bool TScrollView::checkOffsetIfOutrideBoundary()
{
    switch (_direction)
    {
        case VERTICAL:
            if (_offset.y >= 0)
            {
                _offset.y = 0;
                return true;
                
            }else if (_offset.y < -_containerSize.height + _frameRect.size.height)
            {
                _offset.y = -_containerSize.height + _frameRect.size.height;
                return true;
            }
            break;
            
        case HORIZONTAL:
            if (_offset.x >= 0)
            {
                _offset.x = 0;
                return true;
                
            }else if (_offset.x < -_containerSize.width + _frameRect.size.width)
            {
                _offset.x = -_containerSize.width + _frameRect.size.width;
                
                return true;
            }
            
            break;
    }
    
    return false;
}

void TScrollView::startBounceBack()
{
    _bouncing = true;
}

void TScrollView::endBounceBack()
{
    _bouncing = false;
    _bounceOriginalSpeed = 0.0f; // @Unused.
    
    _topBounceNeeded    = false;
    _bottomBounceNeeded = false;
    _leftBounceNeeded   = false;
    _rightBounceNeeded  = false;
}

void TScrollView::addEventListener(const tScrollViewCallback &callback)
{
    _eventCallback = callback;
}


void TScrollView::scrollTo(TScrollView::EventType event)
{
//    this->retain();
    
    if (_eventCallback)
    {
        _eventCallback(this, event);
    }
    
//    this->release();
}

void TScrollView::computePageIndex()
{
    int x = (int)((_containerSize.width - fabs((_containerSize.width + _offset.x))) / _frameRect.size.width);
    
    setCurrentPageIndex(x);
}

void TScrollView::moveToNext()
{
    TLog("TScrollView::moveToNext");
    
    _curPageIndex++;
    float x_to = -(_frameRect.size.width * _curPageIndex);
 
    _offset = Vec2(x_to, 0);
    
    _container->runAction(Sequence::create(MoveTo::create(0.3f, Vec2(x_to, 0)),
                                           CallFunc::create(LAMBDA_FUNCTION_START
                                                            this->scrollTo(TScrollView::EventType::ADJUST_DONE);
                                                            LAMBDA_FUNCTION_END),
                                           nullptr));
}

void TScrollView::moveToPre()
{
    TLog("TScrollView::moveToPre()");
    
    _curPageIndex--;
    float x_to = -(_frameRect.size.width * _curPageIndex);
    
    _offset = Vec2(x_to, 0);
    
    _container->runAction(Sequence::create(MoveTo::create(0.3f, Vec2(x_to, 0)),
                                           CallFunc::create(LAMBDA_FUNCTION_START
                                                            this->scrollTo(TScrollView::EventType::ADJUST_DONE);
                                                            LAMBDA_FUNCTION_END),
                                           nullptr));
}

void TScrollView::moveToCurPageOriginPos()
{
    TLog("TScrollView::moveToCurPageOriginPos()");
    
    float x_to = -_frameRect.size.width * _curPageIndex;
    
    _offset = Vec2(x_to, 0);
    
    _container->runAction(Sequence::create(MoveTo::create(0.3f, Vec2(x_to, 0)),
                                           CallFunc::create(LAMBDA_FUNCTION_START
                                                            this->scrollTo(TScrollView::EventType::ADJUST_DONE);
                                                            LAMBDA_FUNCTION_END),
                                           nullptr));
}


void TScrollView::beforeDraw()
{
    //ScrollView don't support drawing in 3D space
    _beforeDrawCommand.init(_globalZOrder);
    _beforeDrawCommand.func = CC_CALLBACK_0(TScrollView::onBeforeDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_beforeDrawCommand);
}

/**
 * clip this view so that outside of the visible bounds can be hidden.
 */
void TScrollView::onBeforeDraw()
{
    if (_clippingToBounds)
    {
        _scissorRestored = false;
        Rect frame = getFrameRect();
        auto glview = Director::getInstance()->getOpenGLView();
        
        if (glview->isScissorEnabled()) {
            _scissorRestored = true;
            _parentScissorRect = glview->getScissorRect();
            //set the intersection of _parentScissorRect and frame as the new scissor rect
            if (frame.intersectsRect(_parentScissorRect)) {
                float x = MAX(frame.origin.x, _parentScissorRect.origin.x);
                float y = MAX(frame.origin.y, _parentScissorRect.origin.y);
                float xx = MIN(frame.origin.x+frame.size.width, _parentScissorRect.origin.x+_parentScissorRect.size.width);
                float yy = MIN(frame.origin.y+frame.size.height, _parentScissorRect.origin.y+_parentScissorRect.size.height);
                glview->setScissorInPoints(x, y, xx-x, yy-y);
            }
        }
        else {
            glEnable(GL_SCISSOR_TEST);
            glview->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
        }
    }
}

void TScrollView::afterDraw()
{
    _afterDrawCommand.init(_globalZOrder);
    _afterDrawCommand.func = CC_CALLBACK_0(TScrollView::onAfterDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_afterDrawCommand);
}

/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void TScrollView::onAfterDraw()
{
    if (_clippingToBounds)
    {
        if (_scissorRestored) {//restore the parent's scissor rect
            auto glview = Director::getInstance()->getOpenGLView();
            
            glview->setScissorInPoints(_parentScissorRect.origin.x, _parentScissorRect.origin.y, _parentScissorRect.size.width, _parentScissorRect.size.height);
        }
        else {
            glDisable(GL_SCISSOR_TEST);
        }
    }
}

void TScrollView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    // quick return if not visible
    if (!isVisible() || !isVisitableByVisitingCamera())
    {
        return;
    }
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
    this->beforeDraw();
    bool visibleByCamera = isVisitableByVisitingCamera();
    
    if (!_children.empty())
    {
        int i=0;
        
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at(i);
            if ( child->getLocalZOrder() < 0 )
            {
                child->visit(renderer, _modelViewTransform, flags);
            }
            else
            {
                break;
            }
        }
        
        // this draw
        if (visibleByCamera)
            this->draw(renderer, _modelViewTransform, flags);
        
        // draw children zOrder >= 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at(i);
            child->visit(renderer, _modelViewTransform, flags);
        }
    }
    else if (visibleByCamera)
    {
        this->draw(renderer, _modelViewTransform, flags);
    }
    
    this->afterDraw();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}


void TScrollView::setScrollViewBarEnable(bool enable)
{
    _scrollBarEnabled = enable;
    
    if (enable)
    {
        initScrollBar();
    }
}


void TScrollView::updateScrollBar(const cocos2d::Vec2 &outOfBoundary)
{
    if (_verticalScrollBar != nullptr)
    {
        _verticalScrollBar->onScrolled(outOfBoundary);
    }
}

void TScrollView::setScrollBarPositionFromCornerForVertical(const cocos2d::Vec2 &positionFromCorner)
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    CCASSERT(_direction != Direction::HORIZONTAL, "Scroll view doesn't have a vertical scroll bar!");
    _verticalScrollBar->setPositionFromCorner(positionFromCorner);
}


Vec2 TScrollView::getScrollBarPositionFromCornerForVertical() const
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    CCASSERT(_direction != Direction::VERTICAL, "Scroll view doesn't have a horizontal scroll bar!");
    return _verticalScrollBar->getPositionFromCorner();
}

void TScrollView::setScrollBarWidth(float width)
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        _verticalScrollBar->setWidth(width);
    }

}

float TScrollView::getScrollBarWidth() const
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        return _verticalScrollBar->getWidth();
    }
    
    return 0;
}

void TScrollView::setScrollBarColor(const cocos2d::Color3B &color)
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        _verticalScrollBar->setColor(color);
    }

}

const Color3B& TScrollView::getScrollBarColor() const
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        return _verticalScrollBar->getColor();
    }
    
    return Color3B::WHITE;
}


void TScrollView::setScrollBarOpacity(GLubyte opacity)
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        _verticalScrollBar->setOpacity(opacity);
    }

}

GLubyte TScrollView::getScrollBarOpacity() const
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        return _verticalScrollBar->getOpacity();
    }

    return -1;
}

void TScrollView::setScrollBarAutoHideEnable(bool autoHideEnable)
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        _verticalScrollBar->setAutoHideEnable(autoHideEnable);
    }

}

bool TScrollView::isScrollBarAutoHideEnable() const
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        return _verticalScrollBar->isAutoHideEnable();
    }

    
    return false;
}

void TScrollView::setScrollBarAutoHideTime(float autoHideTime)
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        _verticalScrollBar->setAutoHideTime(autoHideTime);
    }

}

float TScrollView::getScrollBarAutoHideTime() const
{
    CCASSERT(_scrollBarEnabled, "Scroll bar should be enabled!");
    if(_verticalScrollBar != nullptr)
    {
        return _verticalScrollBar->getAutoHideTime();
    }
    
    return 0;
}

void TScrollView::initScrollBar()
{
    if(_direction == Direction::VERTICAL && _verticalScrollBar == nullptr)
    {
        _verticalScrollBar = TScrollViewBar::create(this, Direction::VERTICAL);
        addChild(_verticalScrollBar, 2);
    }

}

Vec2 TScrollView::getHowMuchOutOfBoundary(const cocos2d::Vec2 &addition) const
{
    Vec2 result = Vec2::ZERO;
    
    float container_left_boundary   = _container->getPositionX() - _container->getAnchorPoint().x * _frameRect.size.width;
    float container_rigth_boundary  = container_left_boundary    + _containerSize.width;
    float container_bottom_boundary = _container->getPositionY() - _container->getAnchorPoint().y * _frameRect.size.height;
    float container_top_boundary    = container_bottom_boundary  + _containerSize.height;
    
    if (container_left_boundary + addition.x > _frameRect.origin.x)
    {
        result.x = _frameRect.origin.x - (container_left_boundary + addition.x);
    }
    else if (container_rigth_boundary + addition.x < _frameRect.size.width)
    {
        result.x = _frameRect.size.width - (container_rigth_boundary + addition.x);
    }
    
    if (container_top_boundary + addition.y < _frameRect.size.height)
    {
        result.y = _frameRect.size.height - (container_top_boundary + addition.y);
    }
    else if (container_bottom_boundary + addition.y > _frameRect.origin.y)
    {
        result.y = _frameRect.origin.y - (container_bottom_boundary + addition.y);
    }
    
    return result;
}

void TScrollView::setScrollBarPositionFromCorner(const cocos2d::Vec2 &positionFromCorner)
{
    if(_direction != Direction::HORIZONTAL)
    {
        setScrollBarPositionFromCornerForVertical(positionFromCorner);
    }
}














