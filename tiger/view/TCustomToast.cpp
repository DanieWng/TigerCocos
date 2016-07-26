//
//  CustomToast.cpp
//  TheSolarSystem
//
//  Created by Wang zhen on 11/25/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#include "TCustomToast.h"

#define MARGIN_TOP_BOTTOM 10
#define MARGIN_LEFT_RIGHT 38

NS_T_BEGIN

USING_NS_CC;

CustomToast::CustomToast()
{
    _contentSize = Size(500, 40);
    _toastMsg    = nullptr;
}

CustomToast::~CustomToast()
{
}

CustomToast* CustomToast::create()
{
    auto pRet = new (std::nothrow) CustomToast();
    
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

CustomToast* CustomToast::create(const std::string &msg)
{
    auto pRet = new (std::nothrow) CustomToast();
    
    if (pRet && pRet->initWithMsg(msg))
    {
        pRet->autorelease();
    }else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool CustomToast::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    auto size = Director::getInstance()->getVisibleSize();
    
    _toastMsg = Label::createWithSystemFont("", "Consolas", 20);
    _toastMsg->setName("");
    _toastMsg->setPosition( Vec2(size.width/2, 40) );
    _toastMsg->setOpacity(0);
    this->addChild(_toastMsg, 1);
    
    _bgLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    _bgLayer->setContentSize(Size(_toastMsg->getDimensions().width + 38, _toastMsg->getDimensions().height + 48));
    _bgLayer->setPosition( Vec2(size.width/2, 20) );
    _bgLayer->setOpacity(0);
    this->addChild(_bgLayer);
    
    return true;
}

bool CustomToast::initWithMsg(const std::string &msg)
{
    if (!Node::init())
    {
        return false;
    }
    
    auto size = Director::getInstance()->getVisibleSize();
    
    _toastMsg = Label::createWithSystemFont(msg, "Consolas", 20);
    _toastMsg->setName(msg);
    _toastMsg->setPosition(Vec2(size.width/2, 40));
    _toastMsg->setOpacity(0);
    this->addChild(_toastMsg, 1);
    
    _bgLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    _bgLayer->setContentSize(Size(_toastMsg->getContentSize().width + MARGIN_LEFT_RIGHT,
                                  _toastMsg->getContentSize().height + MARGIN_TOP_BOTTOM));
    
    _bgLayer->setPosition(Vec2(size.width/2 - _bgLayer->getContentSize().width/2,
                               20));
    
    _bgLayer->setOpacity(0);
    
    this->addChild(_bgLayer);

    return true;
}

void CustomToast::setToastMsg(const std::string& msg)
{
    auto size = Director::getInstance()->getVisibleSize();
    
    _toastMsg->setString(msg);
    
    _bgLayer->setContentSize(Size(_toastMsg->getContentSize().width + MARGIN_LEFT_RIGHT,
                                  _toastMsg->getContentSize().height + MARGIN_TOP_BOTTOM));
    
    _bgLayer->setPosition(Vec2(size.width/2 - _bgLayer->getContentSize().width/2,
                               20));
}

void CustomToast::show()
{
    auto show_act = FadeTo::create(0.5f, 220);
    _bgLayer->runAction(show_act);
    _toastMsg->runAction(FadeIn::create(0.5f));
    
    this->scheduleOnce(schedule_selector(CustomToast::removeSelf), 2.0f);
}

void CustomToast::removeSelf(float dt)
{
    auto hide_act = FadeOut::create(0.5f);
    
    _bgLayer->runAction(hide_act);
    _toastMsg->runAction(Sequence::create(hide_act->clone(),
                                          CallFunc::create(LAMBDA_FUNCTION_START
                                                           this->removeFromParentAndCleanup(true);
                                                           LAMBDA_FUNCTION_END),
                                          nullptr));
    
}

NS_T_END
















