//
//  TigerPolygonButton.cpp
//  NumbersLadyDemo
//
//  Created by Wang zhen on 3/30/16.
//
//

#include "TigerPolygonButton.hpp"
#include "TigerMacros.h"

TigerPolygonButton::TigerPolygonButton():
TigerBaseButton()
{
    _touchListener = nullptr;
    _btnPriority = -1;
    _btnTag = -1;
    
    _touchDelegate = nullptr;
}

TigerPolygonButton::~TigerPolygonButton()
{
    _touchDelegate = nullptr;
    if (_touchListener)
    {
        SAFE_REMOVE_LISTENER(_touchListener);
    }
}

TigerPolygonButton* TigerPolygonButton::create(const std::string& normal,
                                               const std::string& select,
                                               const std::string& disable,
                                               Widget::TextureResType type)
{
    auto ret = new (std::nothrow)TigerPolygonButton();
    
    if (ret && ret->initWithFile(normal,
                                 select,
                                 disable,
                                 type))
    {
        ret->autorelease();
    }else
    {
        CC_SAFE_DELETE(ret);
        ret = nullptr;
    }
    
    return ret;
}

bool TigerPolygonButton::initWithFile(const std::string &normal, const std::string &select, const std::string &disable, Widget::TextureResType type)
{
    if (!TigerBaseButton::initWithFile(normal, select, disable, type))
    {
        return false;
    }
    
    setTouchListenerEnable(true);
    
    return true;
}

void TigerPolygonButton::addCollisionDetectionShape(const std::string &shape, int fixedPriority, int bodyTag)
{
    _btnPriority = fixedPriority;
    _btnTag = bodyTag;
    
    auto body = PhysicsShapeCache::getInstance()->createBodyWithName(shape);
    body->setGroup(fixedPriority); // 设置刚体触摸优先级
    body->setTag(bodyTag);
    this->setPhysicsBody(body);
}

void TigerPolygonButton::setTouchListenerEnable(bool enable)
{
    if (enable)
    {
        if (_touchListener)
        {
            SAFE_REMOVE_LISTENER(_touchListener);
        }
        
        _touchListener = EventListenerTouchOneByOne::create();
        
        _touchListener->setSwallowTouches(true);
        
        _touchListener->onTouchBegan = CC_CALLBACK_2(TigerPolygonButton::onTouchBegan, this);
        _touchListener->onTouchMoved = CC_CALLBACK_2(TigerPolygonButton::onTouchMoved, this);
        _touchListener->onTouchEnded = CC_CALLBACK_2(TigerPolygonButton::onTouchEnded, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    }else
    {
        if (_touchListener)
        {
            SAFE_REMOVE_LISTENER(_touchListener);
        }
    }
}

bool TigerPolygonButton::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
    bool is_touched = false;
    
    auto cur_scene = Director::getInstance()->getRunningScene();
    auto arr = cur_scene->getPhysicsWorld()->getShapes(touch->getLocation());
    
    if (arr.size() > 0)
    {
        bool is_priority_max = false;
        
        for (auto shape : arr)
        {
            if (_btnPriority >= shape->getBody()->getGroup() &&
                _btnTag == shape->getBody()->getTag())
            {
                is_priority_max = true;
            }
            else if (_btnPriority < shape->getBody()->getGroup() &&
                      _btnTag != shape->getBody()->getTag())
            {
                is_priority_max = false;
            }
        }
        
        if (is_priority_max)
        {
            is_touched = true;
            Widget::pushDownEvent();
            RETURN_DELEGATE_1(_touchDelegate, touch);
        }
    }
    
    return is_touched;
}

void TigerPolygonButton::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
    RETURN_DELEGATE_1(_touchDelegate, touch);
    Widget::onTouchMoved(touch, unusedEvent);
}

void TigerPolygonButton::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unusedEvent)
{
    RETURN_DELEGATE_1(_touchDelegate, touch);
    Widget::onTouchEnded(touch, unusedEvent);
}

void TigerPolygonButton::setButtonTouchDelegate(const fButtonTouchDelegate d)
{
    _touchDelegate = d;
}






























