//
//  TSlipSwitchButton.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 1/12/16.
//
//

#include "TSlipSwitchButton.hpp"
#include <cocostudio/CocoStudio.h>
#include <ui/CocosGUI.h>

#define DefaultBallAndBarLeftPadding    13.0f
#define DefaultBallAndBarRightPadding   13.0f          //18.5f

USING_NS_T;
USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;

TSlipSwitchButton::TSlipSwitchButton()
{
    _curSwitchBtnType = SwitchButtonType::kOn;
    _ballLeftPadding = DefaultBallAndBarLeftPadding;
    _ballRightPadding = DefaultBallAndBarRightPadding;
    
    _percent = 0.0f;
    
    _isTouchMoved = false;
    _btnTypeChangeDelegate = nullptr;
    
    _ballNode = nullptr;
    
    _totalDelta = Vec2::ZERO;
}

TSlipSwitchButton::~TSlipSwitchButton()
{
    _btnTypeChangeDelegate = nullptr;
}

bool TSlipSwitchButton::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    return true;
}

void TSlipSwitchButton::addBar(const std::string bgOnRes, const std::string bgOffRes, bool isPlist)
{
    if (!isPlist)
    {
        _barOn = Sprite::create(bgOnRes);
        _barOff = Sprite::create(bgOffRes);
    }else
    {
        _barOn = Sprite::createWithSpriteFrameName(bgOnRes);
        _barOff = Sprite::createWithSpriteFrameName(bgOffRes);
    }
    
    _barOn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _barOff->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    this->addChild(_barOff);
    this->addChild(_barOn);
    
    if (_curSwitchBtnType == SwitchButtonType::kOff)
    {
        _barOn->setOpacity(0);
    }else
    {
        _barOn->setOpacity(255);
    }
}

void TSlipSwitchButton::addButton(const std::string &btnRes, bool isPlist)
{
    if (!_barOn || !_barOff)
    {
        CC_ASSERT("TSlipSwitchButton - Bar is nullptr");
        return;
    }
    
    const Size bar_content_size = _barOn->getContentSize();
    
    if (!isPlist)
    {
        _ballNode = Sprite::create(btnRes);
    }else
    {
        _ballNode = Sprite::createWithSpriteFrameName(btnRes);
    }
    
    _ballNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _ballNode->setPosition(getBallPositionWithSwitchButtonType(_curSwitchBtnType));
    this->addChild(_ballNode);
    
    initMaxTouchMoveLength();
    
    setTouchListenerEnable(true);
}

void TSlipSwitchButton::initMaxTouchMoveLength()
{
    _maxTouchMoveLength = getBallPositionWithSwitchButtonType(Tiger::SwitchButtonType::kOff).x + _ballLeftPadding;
    _unitMoveLength = _maxTouchMoveLength / 100.0f;
    
    CCASSERT(_ballNode!=nullptr, "_ballNode is nullptr");
    
    _percent = getPercentWithBallPosition(_ballNode->getPosition());
}

Vec2 TSlipSwitchButton::getBallPositionWithSwitchButtonType(Tiger::SwitchButtonType type)
{
    Vec2 pos = Vec2::ZERO;
    
    const Size bar_content_size = _barOn->getContentSize();
    
    if (type == Tiger::SwitchButtonType::kOn)
    {
        pos.x = 0 - _ballLeftPadding;
        pos.y = bar_content_size.height / 2.0f;
        
    }else
    {
        if (_ballNode)
        {
            pos.x = bar_content_size.width + _ballRightPadding - _ballNode->getContentSize().width;
            pos.y = bar_content_size.height / 2.0f;
        }
    }
    
    return pos;
}

float TSlipSwitchButton::getPercentWithBallPosition(cocos2d::Vec2 ballPos)
{
    if (ballPos.x < 0)
    {
        ballPos.x = (_ballLeftPadding - fabsf(ballPos.x));
    }else
    {
        ballPos.x += _ballLeftPadding;
    }
    
    _percent = (ballPos.x / _maxTouchMoveLength) * 100;
    
    if (_percent < 0.0f)
    {
        _percent = 0.0f;
    }else if (_percent > 100.0f)
    {
        _percent = 100.0f;
    }
    
//    TLog("TSlipSwitchButton::getPercentWithBallPosition [%f]", _percent);
    
    return _percent;
}

void TSlipSwitchButton::setBallAndBarLeftPadding(float v)
{
    if (!_ballNode)
    {
        CC_ASSERT("ball is nullptr");
    }
    
    _ballLeftPadding = v;
    setBallPosition(getBallPositionWithSwitchButtonType(_curSwitchBtnType));
}

void TSlipSwitchButton::setBallAndBarRightPadding(float v)
{
    if (!_ballNode)
    {
        CC_ASSERT("ball is nullptr");
    }
    
    _ballRightPadding = v;
    setBallPosition(getBallPositionWithSwitchButtonType(_curSwitchBtnType));
    
    initMaxTouchMoveLength();
}

void TSlipSwitchButton::setSwitchBtnType(Tiger::SwitchButtonType type)
{
    if (!_ballNode)
    {
        CC_ASSERT("ball is nullptr");
    }
    
    _curSwitchBtnType = type;
    setBallPosition(getBallPositionWithSwitchButtonType(_curSwitchBtnType));
}

void TSlipSwitchButton::setBallPosition(const cocos2d::Vec2 pos, bool isAnimation)
{
    if (_ballNode)
    {
        if (!isAnimation)
        {
            _ballNode->setPosition(pos);
        }else
        {
            float timer = 0.05f;
            if (!_isTouchMoved)
            {
                timer = 0.3f;
            }
            
            _ballNode->runAction(EaseSineOut::create(MoveTo::create(timer, pos)));
        }
        
        _percent = getPercentWithBallPosition(pos);
        
        _barOn->setOpacity(255 - (255*(_percent/100.0f)));
    }
}

bool TSlipSwitchButton::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    bool is_touched = false;
    
    auto local = this->convertToNodeSpace(touch->getLocation());
    
    auto ball_boundingbox = _ballNode->getBoundingBox();
    if (ball_boundingbox.containsPoint(local))
    {
//        TLog("click ball");
        is_touched = true;
        _totalDelta = Vec2::ZERO;
    }
    
    return is_touched;
}

void TSlipSwitchButton::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto delta = touch->getDelta();
    
    _totalDelta += delta;
    
    Vec2 ball_pos = _ballNode->getPosition();
    if (delta.x > 0.0f)
    {
        ball_pos.x += fabsf(delta.x);
    }else if (delta.x < 0.0f)
    {
        ball_pos.x -= fabsf(delta.x);
    }
    
    if (ball_pos.x < -_ballLeftPadding)
    {
        ball_pos.x = -_ballLeftPadding;
    }else if(ball_pos.x > (_maxTouchMoveLength-_ballLeftPadding))
    {
        ball_pos.x = (_maxTouchMoveLength-_ballLeftPadding);
    }
    
    if (fabsf(_totalDelta.x) > (_ballNode->getContentSize().width/20.0f))
    {
        _isTouchMoved = true;
        setBallPosition(ball_pos);
    }
}

void TSlipSwitchButton::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!_isTouchMoved)
    {
        if (_percent == 0.0f)
        {
            _curSwitchBtnType = SwitchButtonType::kOff;
            setBallPosition(getBallPositionWithSwitchButtonType(_curSwitchBtnType), true);
        }else if (_percent == 100.0f)
        {
            _curSwitchBtnType = SwitchButtonType::kOn;
            setBallPosition(getBallPositionWithSwitchButtonType(_curSwitchBtnType), true);
        }
    }else
    {
        if (_percent >= 50)
        {
            _curSwitchBtnType = SwitchButtonType::kOff;
            setBallPosition(getBallPositionWithSwitchButtonType(_curSwitchBtnType), true);
        }else
        {
            _curSwitchBtnType = SwitchButtonType::kOn;
            setBallPosition(getBallPositionWithSwitchButtonType(_curSwitchBtnType), true);
        }
    }
    
    if (_btnTypeChangeDelegate)
    {
        _btnTypeChangeDelegate(this, _curSwitchBtnType);
    }
    
    _isTouchMoved = false;
}

void TSlipSwitchButton::setTouchListenerEnable(bool enable)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(TSlipSwitchButton::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TSlipSwitchButton::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TSlipSwitchButton::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void TSlipSwitchButton::setSwitchButtonTypeChangeDelegate(const fSwitchButtonTypeChangeDelegate d)
{
    _btnTypeChangeDelegate = d;
}












