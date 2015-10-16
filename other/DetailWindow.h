//
//  DetailWindow.h
//  AVAD
//
//  Created by Wang zhen on 3/17/15.
//
//

#ifndef __AVAD__DetailWindow__
#define __AVAD__DetailWindow__

#include "AppConfig.h"
#include "sqlite3.h"
#include "DetailDataModel.h"

#include "../cocos2d/cocos/ui/UIPageView.h"
#include "../cocos2d/cocos/ui/UIScrollView.h"
#include "DetailContentView.h"

using namespace cocos2d::ui;

enum DetailSocialButtonType
{
    FACEBOOK,
    MESSAGE,
    EMAIL,
    KAKAOTALK,
    CLOSE
};

class DetailWindow : public Layer
{

public:
    
    DetailWindow();
    ~DetailWindow();
    
    static DetailWindow* initDetailWindow(Enums::CategoryType categoryType);
    
    CC_SYNTHESIZE(Enums::CategoryType, m_categortType, CategoryType);
    CC_SYNTHESIZE(bool, m_isOpened, IsOpened);
    
    virtual bool init();
    CREATE_FUNC(DetailWindow);
    
    void initWithCategoryType();
    void initSocialMenu();
    
    void showDetailWindow(const std::string subName);
    void showTodayVerseWindow(const std::string customID);

    void closeDetailWindow();
    
    void update(float dt);
    void updateLocationData();
    
private:
    
    const int   SHADOW_Z_ORDER      = 1;
    const int   WINDOW_Z_ORDER      = 2;
    const int   TITLE_Z_ORDER       = 3;
    const Vec2  WINDOW_POS          = Vec2(16, 200);
    
    const float BTN_MARGIN_L_R      = 55;
    const float TITLE_POS_Y         = 735;
    const float TITLE_HIDE_X        = 20;
    const float FACEBOOK_MARGIN_L   = 100;
    const float SOCIAL_BTN_Y        = 45;
    
    const float PAGE_VIEW_HEIGHT    = 290;
    const float CONTENT_VIEW_MARGIN_L = 18;
    
    const std::string getCategoryResName();
    std::string       m_categoryResName;
    
    const std::string getCategoryName();
    
    Sprite* m_window;
    Sprite* m_title;
    LayerColor* m_shadow;
    
    Menu* m_menus;
    void menuCallback(Ref* sender);
    
    void doMessageEvent();
    void doFacebookEvent();
    void doMailEvent();
    void doKakaoEvent();
    
//    bool m_isClickedFbBtn;
//    bool m_isClickedMsgBtn;
//    bool m_isClickedMailBtn;
//    bool m_isClickedKakaoBtn;
    bool m_isClickedCloseBtn;
    
//    Sprite* m_fbButton;
//    Sprite* m_msgButton;
//    Sprite* m_emailButton;
//    Sprite* m_kakaoButton;
//    Sprite* m_closeButton;
    
//    void checkButtonIsClicked(Vec2 location);
//    void checkButtonIsKeepClicked(Vec2 location);
//    void doButtonEvent(Vec2 location);
    
    // About DB
    sqlite3* m_db;
    
    // About title, page view and scroll view
    Label* m_categoryTitleLable;
    Label* m_contentLocationLable;
    
    void initDetailContent(std::string titleName, std::vector<DetailDataModel> models);
    
    DetailContentView* m_detailContentView;
    
    std::vector<DetailDataModel> m_DetailData_v;
    
    
};

#endif /* defined(__AVAD__DetailWindow__) */
