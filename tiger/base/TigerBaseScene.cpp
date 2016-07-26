//
//  TigerBaseScene.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 1/15/16.
//
//

#include "TigerBaseScene.hpp"
#include "TCustomToast.h"

TigerBaseScene::TigerBaseScene()
{

}

TigerBaseScene::~TigerBaseScene()
{
    _loadPercentdelegate = nullptr;
}

void TigerBaseScene::onEnter()
{
    TigerBaseLayer::onEnter();
    
}

bool TigerBaseScene::init()
{
    if (!TigerBaseLayer::init())
    {
        return false;
    }
    
    _loadPercentdelegate = nullptr;
    _asyncCompletedCount = 0;
    
    preloadEffect();
    
    return true;
}

void TigerBaseScene::setSceneLoadPercentDelegate(const fAsyncLoadPercentDelegate d)
{
    _loadPercentdelegate = d;
    initScene();
}

void TigerBaseScene::setKeyboardListenerEnable(bool enable)
{
    if (enable)
    {
        auto key_listener = EventListenerKeyboard::create();
        key_listener->onKeyReleased = CC_CALLBACK_2(TigerBaseScene::onKeyReleased, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(key_listener, this);
    }
    else
    {
        _eventDispatcher->removeEventListenersForType(EventListener::Type::KEYBOARD);
    }
}

void TigerBaseScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
//    if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
//    {
//        if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        {
//            showExitWarnningMsg();
//        }
//    }
}

void TigerBaseScene::showExitWarnningMsg()
{
    std::string msg = "One more click the back button to exit";
    
    switch (Application::getInstance()->getCurrentLanguage())
    {
        case cocos2d::LanguageType::KOREAN:
            msg = "뒤로 버튼을 한번 더 터치하시면 종료됩니다";
            break;
            
        case cocos2d::LanguageType::JAPANESE:
            msg = "後にボタンをもう一度タッチしたら終了されます";
            break;
            
        default:
            break;
    }
    
    auto toast = Tiger::CustomToast::create();
    toast->setToastMsg(msg);
    toast->show();
    
    this->addChild(toast, 10);
}










