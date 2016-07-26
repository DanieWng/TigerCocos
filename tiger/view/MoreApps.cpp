//
//  MoreApps.cpp
//  TigerAPI
//
//  Created by Wang zhen on 4/4/16.
//
//

#include "MoreApps.hpp"
#include <TigerFunctions.h>
#include <BaseAppConfig.hpp>

MoreApps::MoreApps()
{

}

MoreApps::~MoreApps()
{
    setTouchListenerEnable(false);
}

void MoreApps::onEnter()
{
    TigerBasePopupLayer::onEnter();
    
    Director::getInstance()->getTextureCache()->removeAllTextures();
}

bool MoreApps::init()
{
    if (!TigerBasePopupLayer::init())
    {
        return false;
    }
    
    const Size visible_size = Director::getInstance()->getVisibleSize();
    const float scene_scale = BaseAppConfig::getInstance()->getSceneScaleFactor();
    
    TEXTURE2D_USE_RGB888();
    auto bg = Sprite::create("layer/bg.png");
    IF_NULL_THEN_RETUEN(bg, false);
    Tiger::setPosAndAnchorPointAndTag(bg,
                                      Vec2(visible_size.width/2.0f, visible_size.height/2.0f),
                                      Vec2::ANCHOR_MIDDLE,
                                      0);
    this->addChild(bg);
    TEXTURE2D_USE_RGBA8888();
    
    auto paths = FileUtils::getInstance()->getSearchPaths();
    
    auto twinkle = Sprite::create("layer/twinkle.png");
    IF_NULL_THEN_RETUEN(twinkle, false);
    Tiger::setPosBaseRetina(twinkle,
                            Vec2(38, 1458),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    this->addChild(twinkle);
    
    auto exit_event = [&](Ref *ref, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            this->hide();
        }
    };
    
    auto btn_exit = Button::create("layer/exit_nor.png",
                                   "layer/exit_sel.png",
                                   "layer/exit_nor.png",
                                   Widget::TextureResType::LOCAL);
    IF_NULL_THEN_RETUEN(btn_exit, false);
    Tiger::setPosBaseRetina(btn_exit,
                            Vec2(1856, 1479),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    btn_exit->addTouchEventListener(exit_event);
    this->addChild(btn_exit);
    
    if (!loadMoreAppsData())
    {
        return false;
    }
    
    for (auto icon : _appIconsData)
    {
        auto btn_icon = Button::create(icon._iconRes,
                                       icon._iconRes,
                                       icon._iconRes,
                                       Widget::TextureResType::LOCAL);
        IF_NULL_THEN_RETUEN(btn_icon, false);
        Tiger::setPosBaseRetina(btn_icon,
                                icon._iconPos,
                                icon._iconAnchorPoint,
                                scene_scale/2.0f);
        btn_icon->setName(icon._name);
        btn_icon->addTouchEventListener(CC_CALLBACK_2(MoreApps::iconTouchEvent, this));
        if (icon._name == MOREAPPS_JSON_NULL_ICON)
        {
            Tiger::setButtonIsEnable(btn_icon, false);
        }
        this->addChild(btn_icon, 5);
    }
    
    return true;
}

bool MoreApps::loadMoreAppsData()
{
    std::string file = "moreapps_last_version.json";
    bool is_last_app = false;
    
    if (APP_RELEASE_DATE >= BaseAppConfig::getInstance()->getCurVersionData()._lastAppReleaseDate)
    {
        is_last_app = true;
        file = "moreapps_second_version.json";
    }
    
    if (!FileUtils::getInstance()->isFileExist(file))
    {
        TLog("\n-- Lost moreapps.json file --\n");
        return false;
    }
    
    const std::string data = FileUtils::getInstance()->getStringFromFile(file);
    
    Document document;
    if (!parseJsonData(document, data))
    {
        TLog("\n-- Parse moreapps.json faild --\n");
        return false;
    }
    
    if (document.IsObject())
    {
        if (document.HasMember(MOREAPPS_JSON_MEMBER_ROOT))
        {
            _appIconsData.clear();
            _appLinksData.clear();
            
            for (int i=0; i<document[MOREAPPS_JSON_MEMBER_ROOT].Size(); i++)
            {
                AppIconData icon_data = AppIconData();
                icon_data._name = document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_APP_NAME].GetString();
                icon_data._iconRes = document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_ICON_RES].GetString();
                icon_data._iconPos = Vec2(document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_ICON_POS]["x"].GetDouble(),
                                          document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_ICON_POS]["y"].GetDouble());
                icon_data._iconAnchorPoint = Vec2(document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_ICON_ANCHOR_P]["x"].GetDouble(),
                                                  document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_ICON_ANCHOR_P]["y"].GetDouble());
                icon_data._releaseDate = document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_RELEASE_DATE].GetUint();
                
                _appIconsData.push_back(icon_data);
                
                if (document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_LINKS_ROOT].IsObject())
                {
                    AppLinkData link_data = AppLinkData();
                    link_data._ios = document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_LINKS_ROOT][LINK_IOS].GetString();
                    link_data._googlePlay = document[MOREAPPS_JSON_MEMBER_ROOT][i][MOREAPPS_JSON_OBJECT_LINKS_ROOT][LINK_GOOGLE_PLAY].GetString();
                
                    _appLinksData[icon_data._name] = {link_data};
                }
            }
        }
    }
    return true;
}

void MoreApps::iconTouchEvent(cocos2d::Ref *ref, Widget::TouchEventType type)
{
    auto btn = static_cast<Button*>(ref);
    auto icon_name = btn->getName();
    
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            btn->getVirtualRenderer()->setColor(Color3B(122, 122, 122));
            break;
            
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            std::string url = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            url = _appLinksData.at(icon_name)._ios;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            url = _appLinksData.at(icon_name)._googlePlay;
#endif
            Tiger::openUrl(url);
        }
            break;
            
        default:
            break;
    }
}

void MoreApps::show()
{
    this->setOpacity(0);
    
    TigerBasePopupLayer::show();
    
    this->runAction(Sequence::create(Spawn::create(FadeIn::create(0.15),
                                                   MoveTo::create(0.3, Vec2(0, 0)),
                                                   nullptr),
                                     nullptr));
}

void MoreApps::hide()
{
    const Size visible_size = Director::getInstance()->getVisibleSize();
    
    this->runAction(Sequence::create(Spawn::create(FadeOut::create(0.15),
                                                   MoveTo::create(0.3, Vec2(0, -visible_size.height)),
                                                   nullptr),
                                     CallFunc::create(LAMBDA_FUNCTION_START
                                                      TigerBasePopupLayer::hide();
                                                      LAMBDA_FUNCTION_END),
                                     nullptr));
}














