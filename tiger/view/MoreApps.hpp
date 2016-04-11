//
//  MoreApps.hpp
//  TigerAPI
//
//  Created by Wang zhen on 4/4/16.
//
//

#ifndef MoreApps_hpp
#define MoreApps_hpp

#include "TigerBasePopupLayer.hpp"
#include <ui/CocosGUI.h>

#define MOREAPPS_JSON_MEMBER_ROOT           "apps"
#define MOREAPPS_JSON_OBJECT_APP_NAME       "app_name"
#define MOREAPPS_JSON_OBJECT_ICON_RES       "app_icon_res"
#define MOREAPPS_JSON_OBJECT_ICON_POS       "app_icon_pos"
#define MOREAPPS_JSON_OBJECT_ICON_ANCHOR_P  "app_icon_anchor_point"
#define MOREAPPS_JSON_OBJECT_RELEASE_DATE   "release_date"
#define MOREAPPS_JSON_OBJECT_LINKS_ROOT     "link"
#define LINK_IOS                            "ios"
#define LINK_GOOGLE_PLAY                    "google_play"

#define MOREAPPS_JSON_NULL_ICON "null"

using namespace cocos2d::ui;

struct AppLinkData
{
    std::string _ios;
    std::string _googlePlay;
};

struct AppIconData
{
    std::string _name;
    std::string _iconRes;
    Vec2 _iconPos;
    Vec2 _iconAnchorPoint;
    unsigned int _releaseDate;
};

class MoreApps : public TigerBasePopupLayer
{
public:
    
    MoreApps();
    ~MoreApps();
    
    virtual void onEnter() override;
    
    virtual bool init() override;
    
    CREATE_FUNC(MoreApps);
    
    virtual void show() override;
    virtual void hide() override;
    
private:
    
    bool loadMoreAppsData();
    void iconTouchEvent(Ref *ref, Widget::TouchEventType type);
    
private:

    Sprite *_twinkle;
    
    std::vector<AppIconData> _appIconsData;
    
    std::unordered_map<std::string, AppLinkData> _appLinksData;
};

#endif /* MoreApps_hpp */





