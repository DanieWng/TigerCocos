//
//  TScrollViewBar.cpp
//  TheMillStone
//
//  Created by Wang zhen on 10/19/15.
//
//

#include "TScrollViewBar.hpp"


namespace Tiger {
    
    static const char* HALF_CIRCLE_IMAGE = "iVBORw0KGgoAAAANSUhEUgAAAAwAAAAGCAMAAADAMI+zAAAAJ1BMVEX///////////////////////////////////////////////////9Ruv0SAAAADHRSTlMABgcbbW7Hz9Dz+PmlcJP5AAAAMElEQVR4AUXHwQ2AQAhFwYcLH1H6r1djzDK3ASxUpTBeK/uTCyz7dx54b44m4p5cD1MwAooEJyk3AAAAAElFTkSuQmCC";
    static const char* BODY_IMAGE_1_PIXEL_HEIGHT = "iVBORw0KGgoAAAANSUhEUgAAAAwAAAABCAMAAADdNb8LAAAAA1BMVEX///+nxBvIAAAACklEQVR4AWNABgAADQABYc2cpAAAAABJRU5ErkJggg==";

    static const Color3B DEFAULT_COLOR(52, 65, 87);
    static const float DEFAULT_MARGIN = 20;
    static const float DEFAULT_AUTO_HIDE_TIME = 0.2f;
    static const float DEFAULT_SCROLLBAR_OPACITY = 0.4f;

    static Sprite* createSpriteFromBase64(const char* base64String)
    {
        unsigned char* decoded;
        int length = base64Decode((const unsigned char*) base64String, (unsigned int) strlen(base64String), &decoded);
        
        Image *image = new Image();
        bool imageResult = image->initWithImageData(decoded, length);
        CCASSERT(imageResult, "Failed to create image from base64!");
        free(decoded);
        
        Texture2D *texture = new Texture2D();
        texture->initWithImage(image);
        texture->setAliasTexParameters();
        image->release();
        
        Sprite* sprite = Sprite::createWithTexture(texture);
        texture->release();
        
        return sprite;
    }
    
    
    TScrollViewBar::TScrollViewBar(TScrollView* parent, TScrollView::Direction direction):
    _parent(parent),
    _direction(direction),
    _upperHalfCircle(nullptr),
    _lowerHalfCircle(nullptr),
    _body(nullptr),
    _opacity(255 * DEFAULT_SCROLLBAR_OPACITY),
    _marginFromBoundary(DEFAULT_MARGIN),
    _marginForLength(DEFAULT_MARGIN),
    _touching(false),
    _autoHideEnable(true),
    _autoHideTime(DEFAULT_AUTO_HIDE_TIME),
    _autoHideRemainingTime(0)
    {
        CCASSERT(parent != nullptr, "Parent scroll view must not be null!");
        CCASSERT(direction != TScrollView::Direction::BOTH, "Illegal scroll direction for scroll bar!");
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
    }
    
    TScrollViewBar::~TScrollViewBar()
    {
    
    }
    
    
    TScrollViewBar* TScrollViewBar::create(TScrollView* parent, TScrollView::Direction direction)
    {
        TScrollViewBar* node = new (std::nothrow) TScrollViewBar(parent, direction);
        if (node && node->init())
        {
            node->autorelease();
            return node;
        }
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    
    bool TScrollViewBar::init()
    {
        if (!ProtectedNode::init())
        {
            return false;
        }
        
        _upperHalfCircle = createSpriteFromBase64(HALF_CIRCLE_IMAGE);
        _upperHalfCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        addProtectedChild(_upperHalfCircle);
        
        _lowerHalfCircle = Sprite::createWithTexture(_upperHalfCircle->getTexture(), _upperHalfCircle->getTextureRect(), _upperHalfCircle->isTextureRectRotated());
        _lowerHalfCircle->setScaleY(-1);
        _lowerHalfCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        addProtectedChild(_lowerHalfCircle);
        
        _body = createSpriteFromBase64(BODY_IMAGE_1_PIXEL_HEIGHT);
        _body->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        addProtectedChild(_body);
        
        setColor(DEFAULT_COLOR);
        
        if(_direction == TScrollView::Direction::HORIZONTAL)
        {
            setRotation(90);
        }
        
        if(_autoHideEnable)
        {
            ProtectedNode::setOpacity(0);
        }
        return true;
    }
    
    void TScrollViewBar::setPositionFromCorner(const cocos2d::Vec2 &positionFromCorner)
    {
        if (_direction == TScrollView::Direction::VERTICAL)
        {
            _marginForLength = positionFromCorner.y;
            _marginFromBoundary = positionFromCorner.x;
        }else
        {
            _marginForLength = positionFromCorner.x;
            _marginFromBoundary = positionFromCorner.y;
        }
    }
    
    Vec2 TScrollViewBar::getPositionFromCorner() const
    {
        if (_direction == TScrollView::Direction::VERTICAL)
        {
            return Vec2(_marginFromBoundary, _marginForLength);
        }else
        {
            return Vec2(_marginForLength, _marginFromBoundary);
        }
    }
    
    void TScrollViewBar::setWidth(float width)
    {
        float scale = width / _body->getContentSize().width;
        _body->setScaleX(scale);
        _upperHalfCircle->setScale(scale);
        _lowerHalfCircle->setScale(-scale);
    }
    
    float TScrollViewBar::getWidth() const
    {
        return _body->getBoundingBox().size.width;
    }
    
    void TScrollViewBar::updateLength(float length)
    {
        float ratio = length / _body->getTextureRect().size.height;
        _body->setScaleY(ratio);
        _upperHalfCircle->setPositionY(_body->getPositionY() + length);
    }
    
    void TScrollViewBar::onEnter()
    {
#if CC_ENABLE_SCRIPT_BINDING
        if (_scriptType == kScriptTypeJavascript)
        {
            if (ScriptEngineManager::sendNodeEventToJSExtended(this, kNodeOnEnter))
            {
                return;
            }
        }
#endif
        
        ProtectedNode::onEnter();
        scheduleUpdate();
    }
    
    void TScrollViewBar::update(float deltaTime)
    {
        processAutoHide(deltaTime);
    }
    
    void TScrollViewBar::processAutoHide(float deltaTime)
    {
        if(!_autoHideEnable || _autoHideRemainingTime <= 0)
        {
            return;
        }
        else if(_touching)
        {
            // If it is touching, don't auto hide.
            return;
        }
        
        _autoHideRemainingTime -= deltaTime;
        if(_autoHideRemainingTime <= _autoHideTime)
        {
            _autoHideRemainingTime = MAX(0, _autoHideRemainingTime);
            ProtectedNode::setOpacity(_opacity * (_autoHideRemainingTime / _autoHideTime));
        }
    }
    
    void TScrollViewBar::onTouchBegan()
    {
        if(!_autoHideEnable)
        {
            return;
        }
        _touching = true;
    }
    
    void TScrollViewBar::onTouchEnded()
    {
        if(!_autoHideEnable)
        {
            return;
        }
        _touching = false;
        
        if(_autoHideRemainingTime <= 0)
        {
            // If the remaining time is 0, it means that it didn't moved after touch started so scroll bar is not showing.
            return;
        }
        _autoHideRemainingTime = _autoHideTime;
    }
    
    void TScrollViewBar::onScrolled(const Vec2& outOfBoundary)
    {
        if(_autoHideEnable)
        {
            _autoHideRemainingTime = _autoHideTime;
            ProtectedNode::setOpacity(_opacity);
        }
        
        auto* innerContainer = _parent->getContainer();
        
        float innerContainerMeasure = 0;
        float scrollViewMeasure = 0;
        float outOfBoundaryValue = 0;
        float innerContainerPosition = 0;
        if(_direction == TScrollView::Direction::VERTICAL)
        {
            innerContainerMeasure = innerContainer->getContentSize().height;
            scrollViewMeasure = _parent->getContentSize().height;
            outOfBoundaryValue = outOfBoundary.y;
            innerContainerPosition = -innerContainer->getPositionY();
        }
        else if(_direction == TScrollView::Direction::HORIZONTAL)
        {
            innerContainerMeasure = innerContainer->getContentSize().width;
            scrollViewMeasure = _parent->getContentSize().width;
            outOfBoundaryValue = outOfBoundary.x;
            innerContainerPosition = -innerContainer->getPositionX();
        }
        
        float length = calculateLength(innerContainerMeasure, scrollViewMeasure, outOfBoundaryValue);
        Vec2 position = calculatePosition(innerContainerMeasure, scrollViewMeasure, innerContainerPosition, outOfBoundaryValue, length);
        updateLength(length);
        setPosition(position);
    }
    
    float TScrollViewBar::calculateLength(float innerContainerMeasure, float scrollViewMeasure, float outOfBoundaryValue)
    {
        float denominatorValue = innerContainerMeasure;
        if(outOfBoundaryValue != 0)
        {
            // If it is out of boundary, the length of scroll bar gets shorter quickly.
            static const float GETTING_SHORTER_FACTOR = 20;
            denominatorValue += (outOfBoundaryValue > 0 ? outOfBoundaryValue : -outOfBoundaryValue) * GETTING_SHORTER_FACTOR;
        }
        
        float lengthRatio = scrollViewMeasure / denominatorValue;
        return fabsf(scrollViewMeasure - 2 * _marginForLength) * lengthRatio;
    }
    
    Vec2 TScrollViewBar::calculatePosition(float innerContainerMeasure, float scrollViewMeasure, float innerContainerPosition, float outOfBoundaryValue, float length)
    {
        float denominatorValue = innerContainerMeasure - scrollViewMeasure;
        if(outOfBoundaryValue != 0)
        {
            denominatorValue += fabs(outOfBoundaryValue);
        }
        
        float positionRatio = 0;
        if(denominatorValue != 0)
        {
            positionRatio = innerContainerPosition / denominatorValue;
            positionRatio = MAX(positionRatio, 0);
            positionRatio = MIN(positionRatio, 1);
        }
        float position = (scrollViewMeasure - length - 2 * _marginForLength) * positionRatio + _marginForLength;
        if(_direction == TScrollView::Direction::VERTICAL)
        {
            return Vec2(_parent->getContentSize().width - _marginFromBoundary, position);
        }
        else
        {
            return Vec2(position, _marginFromBoundary);
        }
    }
    
}





























