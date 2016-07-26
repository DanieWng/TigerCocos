//
//  TWebView.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 5/27/16.
//
//

#include "TWebView.hpp"
#include "BaseAppConfig.hpp"

TWebView::~TWebView()
{
    _webView->setVisible(false);
}

bool TWebView::init()
{
    if (!TigerBaseLayer::init())
    {
        return false;
    }
    
    auto touch_listener = EventListenerTouchOneByOne::create();
    touch_listener->setSwallowTouches(true);
    touch_listener->onTouchBegan = [&](Touch *t, Event *e)->bool{
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    auto bg = LayerColor::create(Color4B::WHITE);
    this->addChild(bg);
    
    const Size visible_size = Director::getInstance()->getVisibleSize();
    const float scene_scale = BaseAppConfig::getInstance()->getSceneScaleFactor();
    
    Rect menu_bar_rect = Rect(0, 0, visible_size.width, 170*(scene_scale/2.0f));
    auto menu_bar = Sprite::create();
    menu_bar->setTextureRect(menu_bar_rect);
    menu_bar->setColor(Color3B(0, 191, 183));
    menu_bar->setPosition(Vec2(0, visible_size.height));
    menu_bar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    this->addChild(menu_bar, 5);
    
    auto btn_back = Button::create("common/web/bar_btn_exit.png",
                                   "common/web/bar_btn_exit.png",
                                   "common/web/bar_btn_exit.png");
    btn_back->addTouchEventListener([&](Ref* ref, Widget::TouchEventType type)->void{
        switch (type)
        {
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                static_cast<Button*>(ref)->getVirtualRenderer()->setColor(Color3B(122, 122, 122));
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                this->setVisible(false);
                this->removeFromParentAndCleanup(true);
                break;
                
            default:
                break;
        }
    });
    btn_back->setPosition(Vec2(visible_size.width-10*scene_scale, 7*scene_scale));
    btn_back->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    menu_bar->addChild(btn_back);
    
    _webView = cocos2d::experimental::ui::WebView::create();
    _webView->setPosition(Vec2(0, visible_size.height-menu_bar_rect.size.height));
    _webView->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _webView->setContentSize(Size(visible_size.width, visible_size.height-menu_bar_rect.size.height));
    _webView->setOnDidFinishLoading(CC_CALLBACK_2(TWebView::loadFinishedEvent, this));
    _webView->setOnShouldStartLoading(CC_CALLBACK_2(TWebView::shouldStartLoading, this));
    _webView->setScalesPageToFit(true);
    this->addChild(_webView);
    
    _loading = Sprite::create("loading/trans_loading_1.png");
    _loading->setPosition(Vec2(visible_size.width/2, visible_size.height/2));
    _loading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loading->setName(WEB_LOADING_NAME);
    _loading->setVisible(false);
    this->addChild(_loading, 20);
    
    return true;
}

void TWebView::loadUrl(const std::string url)
{
    _webView->loadURL(url);
}

void TWebView::loadFinishedEvent(experimental::ui::WebView *sender, const std::string &url)
{
    CCLOG("load url[%s] -- sucessed", url.c_str());
    
    if (_loading)
    {
        _loading->setVisible(false);
        _webView->setVisible(true);
    }
}

bool TWebView::shouldStartLoading(experimental::ui::WebView *sender, const std::string &url)
{
    CCLOG("should start loading [%s]", url.c_str());

    if (_loading)
    {
        playLoadingAnimation();
        _webView->setVisible(false);
    }
    
    return true;
}

void TWebView::playLoadingAnimation()
{
    if (_loading->isRunning())
    {
        _loading->stopAllActions();
    }
    
    auto p_anim = Animation::create();
    p_anim->setLoops(-1);
    p_anim->setDelayPerUnit(1/7.0f);
    p_anim->setRestoreOriginalFrame(true);
    p_anim->addSpriteFrameWithFile("loading/trans_loading_1.png");
    p_anim->addSpriteFrameWithFile("loading/trans_loading_2.png");
    p_anim->addSpriteFrameWithFile("loading/trans_loading_3.png");
    p_anim->addSpriteFrameWithFile("loading/trans_loading_4.png");

    _loading->setVisible(true);
    _loading->setSpriteFrame(p_anim->getFrames().at(0)->getSpriteFrame());
    _loading->runAction(Animate::create(p_anim));
}


