//
//  DetailWindow.cpp
//  AVAD
//
//  Created by Wang zhen on 3/17/15.
//
//

#include "DetailWindow.h"
#include "TigerSqliteUtil.h"
#include "AlarmWindow.h"

#define GOOGLE_PLAY_URL  "https://goo.gl/jJnYcU"
#define ITUNES_STORE_URL "https://goo.gl/yPuiNj"

DetailWindow::DetailWindow():
m_categoryResName(""),
m_isOpened(false),
m_db(nullptr),
//m_isClickedFbBtn(false),
//m_isClickedMsgBtn(false),
//m_isClickedMailBtn(false),
//m_isClickedKakaoBtn(false),
m_isClickedCloseBtn(false),
m_detailContentView(nullptr),
m_categoryTitleLable(nullptr),
m_contentLocationLable(nullptr)
{

}

DetailWindow::~DetailWindow()
{

}

DetailWindow* DetailWindow::initDetailWindow(Enums::CategoryType categoryType)
{
    auto detail_window = DetailWindow::create();
    detail_window->setCategoryType(categoryType);
    detail_window->initWithCategoryType();
    
    return detail_window;
}

bool DetailWindow::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

void DetailWindow::initWithCategoryType()
{
    auto win_size = Director::getInstance()->getVisibleSize();
    
    m_categoryResName = getCategoryResName();
    
    m_window = Sprite::create("detail/verse_bg.png");
    setPosAndAnchorPointAndTag(m_window,
                               Vec2(win_size.width, WINDOW_POS.y),
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               0);
    this->addChild(m_window, WINDOW_Z_ORDER);
    m_window->setVisible(false);
    
    m_title = Sprite::create(StringUtils::format("detail/verse_bar%s.png", m_categoryResName.c_str()));
    setPosAndAnchorPointAndTag(m_title,
                               Vec2(-m_title->getContentSize().width, TITLE_POS_Y),
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               0);
    this->addChild(m_title, TITLE_Z_ORDER);
    m_title->setVisible(false);
    
    m_shadow = LayerColor::create(Color4B(0, 0, 0, 122));
    setPosAndAnchorPointAndTag(m_shadow,
                               Vec2::ZERO,
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               0);
    this->addChild(m_shadow, SHADOW_Z_ORDER);
    m_shadow->setVisible(false);
    
    /*
     Create social menu.
     */
    initSocialMenu();
}

const std::string DetailWindow::getCategoryResName()
{
    std::string name;
    switch (m_categortType)
    {
        case Enums::TODAYS_VERSE:
            name = "_today";
            break;
            
        case Enums::CHRIST:
            name = "_christ";
            break;
            
        case Enums::FAITH:
            name = "_faith";
            break;
            
        case Enums::WE:
            name = "_we";
            break;
            
        case Enums::I:
            name = "_i";
            break;
    }
    
    return name;
}

const std::string DetailWindow::getCategoryName()
{
    std::string name;
    switch (m_categortType)
    {
        case Enums::TODAYS_VERSE:
            name = AppConfig::_M_LANGUAGE_ == Enums::ENGLISH ? "Today" : "오늘의 말씀";
            break;
            
        case Enums::CHRIST:
            name = AppConfig::_M_LANGUAGE_ == Enums::ENGLISH ? "Christ" : "그리스도";
            break;
            
        case Enums::FAITH:
            name = AppConfig::_M_LANGUAGE_ == Enums::ENGLISH ? "Faith" : "믿음";
            break;
            
        case Enums::WE:
            name = AppConfig::_M_LANGUAGE_ == Enums::ENGLISH ? "We" : "우리";
            break;
            
        case Enums::I:
            name = AppConfig::_M_LANGUAGE_ == Enums::ENGLISH ? "I" : "나";
            break;
    }
    
    TLog("DetailWindow::getCategoryName() --> %s", name.c_str());
    
    return name;
}


void DetailWindow::initSocialMenu()
{
    auto res_name = m_categoryResName.c_str();
    
    auto fb_n = Sprite::create(StringUtils::format("detail/bt%s_facebook.png", res_name));
    auto fb_s = Sprite::create(StringUtils::format("detail/bt%s_facebook.png", res_name));
    fb_s->setOpacity(122);
    auto fb = MenuItemSprite::create(fb_n,
                                     fb_s,
                                     CC_CALLBACK_1(DetailWindow::menuCallback, this));
    setPosAndAnchorPointAndTag(fb,
                               Vec2(FACEBOOK_MARGIN_L, SOCIAL_BTN_Y),
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               DetailSocialButtonType::FACEBOOK);

    auto msg_n = Sprite::create(StringUtils::format("detail/bt%s_sms.png", res_name));
    auto msg_s = Sprite::create(StringUtils::format("detail/bt%s_sms.png", res_name));
    msg_s->setOpacity(122);
    auto msg = MenuItemSprite::create(msg_n,
                                      msg_s,
                                      CC_CALLBACK_1(DetailWindow::menuCallback, this));
    setPosAndAnchorPointAndTag(msg,
                               Vec2(fb->getPositionX() + fb->getContentSize().width + BTN_MARGIN_L_R, SOCIAL_BTN_Y),
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               DetailSocialButtonType::MESSAGE);

    auto email_n = Sprite::create(StringUtils::format("detail/bt%s_mail.png", res_name));
    auto email_s = Sprite::create(StringUtils::format("detail/bt%s_mail.png", res_name));
    email_s->setOpacity(122);
    auto email = MenuItemSprite::create(email_n,
                                        email_s,
                                        CC_CALLBACK_1(DetailWindow::menuCallback, this));
    setPosAndAnchorPointAndTag(email,
                               Vec2(msg->getPositionX() + msg->getContentSize().width + BTN_MARGIN_L_R, SOCIAL_BTN_Y+4),
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               DetailSocialButtonType::EMAIL);

    auto kakao_n = Sprite::create(StringUtils::format("detail/bt%s_kakaotalk.png", res_name));
    auto kakao_s = Sprite::create(StringUtils::format("detail/bt%s_kakaotalk.png", res_name));
    kakao_s->setOpacity(122);
    auto kakao = MenuItemSprite::create(kakao_n,
                                        kakao_s,
                                        CC_CALLBACK_1(DetailWindow::menuCallback, this));
    setPosAndAnchorPointAndTag(kakao,
                               Vec2(email->getPositionX() + email->getContentSize().width + BTN_MARGIN_L_R, SOCIAL_BTN_Y),
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               DetailSocialButtonType::KAKAOTALK);
    
    /*
     If in iOS device and the kakao talk app isn't installed,
     then need to set kakao item be unvisiable and reset other items position.
     */
    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && !TigerObjectCCalls::tryCheckIsKakaoInstalled())
    {
        TLog("\n-----------------------------------");
        TLog("Is not installed The kakao talk app.");
        TLog("-----------------------------------\n");
        
        kakao->setVisible(false);
        
        float kakao_width = kakao->getContentSize().width;
        
        fb->setPositionX(fb->getPositionX() + kakao_width - 10);
        msg->setPositionX(msg->getPositionX() + kakao_width - 10);
        email->setPositionX(email->getPositionX() + kakao_width - 10);
    }
    
    auto close_n = Sprite::create("detail/bt_verse_close.png");
    auto close_s = Sprite::create("detail/bt_verse_close.png");
    close_s->setOpacity(122);
    auto close = MenuItemSprite::create(close_n,
                                        close_s,
                                        CC_CALLBACK_1(DetailWindow::menuCallback, this));
    setPosAndAnchorPointAndTag(close,
                               Vec2(m_window->getContentSize().width  - close->getContentSize().width  - BTN_MARGIN_L_R,
                                    m_window->getContentSize().height - close->getContentSize().height - BTN_MARGIN_L_R/2),
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               DetailSocialButtonType::CLOSE);
    
    m_menus = Menu::create(fb, msg, email, kakao, close, nullptr);
    setPosAndAnchorPointAndTag(m_menus,
                               Vec2::ZERO,
                               Vec2::ANCHOR_BOTTOM_LEFT,
                               0);
    m_window->addChild(m_menus, WINDOW_Z_ORDER);
    
}

void DetailWindow::initDetailContent(std::string titleName, std::vector<DetailDataModel> models)
{
    /*----------------------------------------------------------------------------------/
        init detail content window's title
     */
    auto title_content_size = m_title->getContentSize();
    
    m_categoryTitleLable = Label::createWithSystemFont(titleName, "Arial", 33);
    setPosAndAnchorPointAndTag(m_categoryTitleLable,
                               Vec2(TITLE_HIDE_X + 10, title_content_size.height - 10),
                               Vec2::ANCHOR_TOP_LEFT,
                               0);
    m_title->addChild(m_categoryTitleLable, TITLE_Z_ORDER);
    
    m_categoryTitleLable->setWidth(190);
    m_categoryTitleLable->setHeight(76);

    m_categoryTitleLable->setLineBreakWithoutSpace(true);
    
    m_categoryTitleLable->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    m_categoryTitleLable->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    
    m_contentLocationLable = Label::createWithSystemFont(models.at(0).data_location, "Arial", 25);
    setPosAndAnchorPointAndTag(m_contentLocationLable,
                               Vec2(title_content_size.width - 20, title_content_size.height - 10),
                               Vec2::ANCHOR_TOP_RIGHT,
                               0);
    m_title->addChild(m_contentLocationLable, TITLE_Z_ORDER);
    
    m_contentLocationLable->setWidth(90);
    m_contentLocationLable->setHeight(76);
    
    m_contentLocationLable->setLineBreakWithoutSpace(true);
    
    m_contentLocationLable->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
    m_contentLocationLable->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    
    /*----------------------------------------------------------------------------------*/
    
    
    m_detailContentView= DetailContentView::create();
    float content_view_width = m_window->getContentSize().width - 32;
    m_detailContentView->setContentViewSize(Size(content_view_width, PAGE_VIEW_HEIGHT));
    
    TLog("detail content view size: %f, %f", m_detailContentView->getContentSize().width, m_detailContentView->getContentSize().height);
    
    m_detailContentView->initContentView(&models);

    m_detailContentView->setPosition( Vec2(22, m_window->getContentSize().height/3) );
    
    m_window->addChild(m_detailContentView, WINDOW_Z_ORDER);
    
}

void DetailWindow::showDetailWindow(const std::string subName)
{
    
    if (AppConfig::_IS_BGM_ENABLE_)
    {
        SimpleAudioEngine::getInstance()->playEffect("sound/detail_show.mp3");
    }
    
    m_title->setVisible(true);
    m_window->setVisible(true);
    m_shadow->setVisible(true);
    m_isOpened = true;
    
    this->scheduleUpdate();
    
    m_title->runAction( EaseExponentialOut::create(MoveTo::create(0.3f, Vec2(0-TITLE_HIDE_X, TITLE_POS_Y))) );
    m_window->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.5f, WINDOW_POS)),
                                         CallFunc::create(LAMBDA_FUNCTION_START
                                                          //
                                                          LAMBDA_FUNCTION_END),
                                         nullptr));
    
    if (strcmp(subName.c_str(), "") == 0)
    {
        return;
    }
    
    std::vector<DetailDataModel> models_v;
    models_v.clear();
    
    /*
     Open database to get verse data.
     */
    std::string db_path = AppConfig::getInstance()->getDBFilePath();
    std::string sql;
    
    auto tiger_sqlite_util = TigerSqliteUtil::getInstance();
    
    tiger_sqlite_util->openDatabaseWithFile(db_path);
    
    sql = StringUtils::format("SELECT Zverse, Zlocation  FROM ZAVADData WHERE Zcategory=\"%s\" ORDER BY Z_PK ASC", subName.c_str());
    
    TLog("DetailWindow::showDetailWindow --> SQL: %s", sql.c_str());
    
    SqliteQueryData data = tiger_sqlite_util->queryWithSQL(sql);
    
    for (int i=1; i<=data.r; i++)
    {
        
        DetailDataModel data_model = {data.re[i*data.c], data.re[i*data.c+1], i};
        models_v.push_back(data_model);
        
    }
    
    tiger_sqlite_util->freeTableWithResult(data.re);
    tiger_sqlite_util->closeDataBase();
    
    // init detail content
    initDetailContent(subName, models_v);
}

void DetailWindow::showTodayVerseWindow(const std::string customID)
{
    TLog("DetailWindow::showTodayVerseWindow --> custom ID: %s", customID.c_str());
    
    if (AppConfig::_IS_BGM_ENABLE_)
    {
        SimpleAudioEngine::getInstance()->playEffect("sound/detail_show.mp3");
    }
    
    if (strcmp(customID.c_str(), "") == 0)
    {
        return;
    }
    
    TLog("\n-----------------------------------------\n");
    auto ud = UserDefault::getInstance();
    
    std::string current_date = getCurrentDate();
    auto last_date = ud->getStringForKey(LAST_DATE_KEY);
    
    
    TLog("current date: %s", current_date.c_str());
    TLog("last date: %s",    last_date.c_str());
    
    if (current_date != ud->getStringForKey(LAST_DATE_KEY))
    {
        TLog("## Its new day, will get new content ##");
        
        AppConfig::m_currentCustomID = AlarmWindow::getNextZcustomIDForTodayVerse();
        TLog("Next today verse's ZcustomID: %d", AppConfig::m_currentCustomID);
        
        ud->setIntegerForKey(LAST_CUSTOM_ID_KEY, AppConfig::m_currentCustomID);
        ud->setStringForKey(LAST_DATE_KEY, current_date);
        ud->flush();
    }else
    {
        TLog("## Its same day, will use same content ##");
    }
    
    TLog("\n-----------------------------------------\n");
    
    
    /*
     Open database to get verse data.
     */
    std::string db_path = AppConfig::getInstance()->getDBFilePath();
    std::string category_name;
    std::string sql;
    
    m_DetailData_v.clear();
    
    auto tiger_sqlite_util = TigerSqliteUtil::getInstance();
    
    tiger_sqlite_util->openDatabaseWithFile(db_path);
    
    sql = StringUtils::format("SELECT Zverse, Zlocation, Zcategory FROM ZAVADData WHERE ZcustomID=\"%s\"", customID.c_str());
    
    SqliteQueryData data = tiger_sqlite_util->queryWithSQL(sql);
    
    for (int i=1; i<=data.r; i++)
    {
        DetailDataModel data_model = {data.re[i*data.c], data.re[i*data.c+1], i};
        category_name = data.re[i*data.c+2];
        m_DetailData_v.push_back(data_model);
    }
    
    tiger_sqlite_util->freeTableWithResult(data.re);
    tiger_sqlite_util->closeDataBase();
    
    // init detail content
    initDetailContent(category_name, m_DetailData_v);
    
    m_title->setVisible(true);
    m_window->setVisible(true);
    m_shadow->setVisible(true);
    m_isOpened = true;
    
    m_title->runAction( EaseExponentialOut::create(MoveTo::create(0.3f, Vec2(0-TITLE_HIDE_X, TITLE_POS_Y))) );
    m_window->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.5f, WINDOW_POS)),
                                         CallFunc::create(LAMBDA_FUNCTION_START
                                                          //
                                                          LAMBDA_FUNCTION_END),
                                         nullptr));
    
    

}

void DetailWindow::closeDetailWindow()
{
    if (AppConfig::_IS_BGM_ENABLE_)
    {
        SimpleAudioEngine::getInstance()->playEffect("sound/detail_close.mp3");
    }
    
    auto win_size = Director::getInstance()->getVisibleSize();
    
    this->unscheduleUpdate();
    
    m_title->runAction(EaseExponentialIn::create(MoveTo::create(0.3f, Vec2(-m_title->getContentSize().width, TITLE_POS_Y))));
    m_window->runAction(Sequence::create(EaseExponentialIn::create(MoveTo::create(0.5f, Vec2(win_size.width, WINDOW_POS.y))),
                                         CallFunc::create(LAMBDA_FUNCTION_START
                                                          m_isOpened = false;
                                                          m_shadow->setVisible(false);
                                                          m_isClickedCloseBtn = false;
                                                          LAMBDA_FUNCTION_END),
                                         nullptr));
    
    // clear all data
    if (m_categoryTitleLable)
    {
        m_categoryTitleLable->removeFromParentAndCleanup(true);
        m_categoryTitleLable = nullptr;
    }

    if (m_contentLocationLable)
    {
        m_contentLocationLable->removeFromParentAndCleanup(true);
        m_contentLocationLable = nullptr;
    }
    
    if (m_detailContentView)
    {
        m_detailContentView->removeFromParentAndCleanup(true);
        m_detailContentView = nullptr;
    }

}

void DetailWindow::menuCallback(cocos2d::Ref *sender)
{
    if (!m_isOpened || m_isClickedCloseBtn)
    {
        return;
    }
    
    if (AppConfig::_IS_BGM_ENABLE_)
    {
        SimpleAudioEngine::getInstance()->playEffect("sound/button.mp3");
    }
    
    auto item = static_cast<MenuItemSprite*>(sender);
    int tag = item->getTag();
    
    switch (tag)
    {
        case DetailSocialButtonType::FACEBOOK:
            TLog("facebook");
            doFacebookEvent();
            break;
            
        case DetailSocialButtonType::MESSAGE:
            TLog("message");
            doMessageEvent();
            break;
            
        case DetailSocialButtonType::EMAIL:
            TLog("email");
            doMailEvent();
            break;
            
        case DetailSocialButtonType::KAKAOTALK:
            TLog("kakao");
            doKakaoEvent();
            break;
            
        case DetailSocialButtonType::CLOSE:
            TLog("close");
            m_isClickedCloseBtn = true;
            AppConfig::_IS_SHARE_ = false;
            closeDetailWindow();
            break;
            
        default:
            break;
    }
}

/**
 Message body
 1. location(...)
 2. current content
 3. app url
 */
void DetailWindow::doMessageEvent()
{
    std::string url = __String::createWithFormat("Google Play: %s\nApp Store: %s\n", GOOGLE_PLAY_URL, ITUNES_STORE_URL)->getCString();
    
    DetailDataModel data = m_detailContentView->getCurrentDataModel();
    
    std::string body = StringUtils::format("(%s)\n%s\n\n%s", data.data_location.c_str(), data.data_verse.c_str(), url.c_str());
    
    TLog("\nDetailWindow::doMessageEvent -->\n%s\n", body.c_str());
    
    TigerFunctions::tryCallMessageFunction(body.c_str());
}

void DetailWindow::doMailEvent()
{
    std::string address = "";
    std::string title   = "";
    
    std::string url = __String::createWithFormat("Google Play: %s\nApp Store: %s\n", GOOGLE_PLAY_URL, ITUNES_STORE_URL)->getCString();
    
    DetailDataModel data = m_detailContentView->getCurrentDataModel();
    
    std::string body = StringUtils::format("(%s)\n%s\n\n\n%s", data.data_location.c_str(), data.data_verse.c_str(), url.c_str());
    
    TLog("\nDetailWindow::doMessageEvent -->\n%s\n", body.c_str());
    
    TigerFunctions::tryCallMailFunction(address.c_str(), title.c_str(), body.c_str());

}

void DetailWindow::doKakaoEvent()
{
    AppConfig::_IS_SHARE_ = true;
    
    std::string url = __String::createWithFormat("Google Play: %s\nApp Store: %s\n", GOOGLE_PLAY_URL, ITUNES_STORE_URL)->getCString();
    
    DetailDataModel data = m_detailContentView->getCurrentDataModel();
    
    std::string body = StringUtils::format("(%s)\n%s\n\n%s", data.data_location.c_str(), data.data_verse.c_str(), url.c_str());
    
    TLog("\nDetailWindow::doKakaoEvent -->\n%s\n", body.c_str());
    
    TigerFunctions::tryCallShareToKakaoTalkWithLabel(body.c_str());
}

void DetailWindow::doFacebookEvent()
{
    AppConfig::_IS_SHARE_ = true;
    
    DetailDataModel data = m_detailContentView->getCurrentDataModel();
    
    std::string location = data.data_location;
    std::string description = data.data_verse;
    
    TigerFunctions::tryCallShareToFackbookWithLabel(location.c_str(), description.c_str());
}

void DetailWindow::updateLocationData()
{
    std::string local_info = (m_detailContentView->getCurrentDataModel()).data_location;
    TLog("DetailWindow::updateLocationData --> %s", local_info.c_str());
    m_contentLocationLable->setString(local_info);
}

void DetailWindow::update(float dt)
{
    if (m_isOpened)
    {
        if (m_detailContentView->getIsPageChanged())
        {
            updateLocationData();
            
            m_detailContentView->setIsPageChanged(false);
        }
    }
}



/*--------------------------------- unused method ---------------------------------*/

//void DetailWindow::checkButtonIsClicked(Vec2 location)
//{
//    if (m_fbButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//    {
//        m_isClickedFbBtn = true;
//        m_fbButton->setOpacity(122);
//        
//    }else if (m_msgButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//    {
//        m_isClickedMsgBtn = true;
//        m_msgButton->setOpacity(122);
//        
//    }else if (m_emailButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//    {
//        m_isClickedMailBtn = true;
//        m_emailButton->setOpacity(122);
//        
//    }else if (m_kakaoButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//    {
//        m_isClickedKakaoBtn = true;
//        m_kakaoButton->setOpacity(122);
//        
//    }else if (m_closeButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//    {
//        m_isClickedCloseBtn = true;
//        m_closeButton->setOpacity(122);
//        
//    }
//}
//
//void DetailWindow::checkButtonIsKeepClicked(cocos2d::Vec2 location)
//{
//    if (m_isClickedFbBtn)
//    {
//        if (!m_fbButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            m_isClickedFbBtn = false;
//            m_fbButton->setOpacity(255);
//        }
//    }else if (m_isClickedMsgBtn)
//    {
//        if (!m_msgButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            m_isClickedMsgBtn = false;
//            m_msgButton->setOpacity(255);
//        }
//    }else if (m_isClickedMailBtn)
//    {
//        if (!m_emailButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            m_isClickedMailBtn = false;
//            m_emailButton->setOpacity(255);
//        }
//    }else if (m_isClickedKakaoBtn)
//    {
//        if (!m_kakaoButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            m_isClickedKakaoBtn = false;
//            m_kakaoButton->setOpacity(255);
//        }
//    }
//}
//
//void DetailWindow::doButtonEvent(cocos2d::Vec2 location)
//{
//    if (m_isClickedFbBtn)
//    {
//        if (m_fbButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            TLog("clicked detail social fackbook button");
//            m_isClickedFbBtn = false;
//            m_fbButton->setOpacity(255);
//        }
//    }else if (m_isClickedMsgBtn)
//    {
//        if (m_msgButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            TLog("clicked detail social message button");
//            m_isClickedMsgBtn = false;
//            m_msgButton->setOpacity(255);
//        }
//    }else if (m_isClickedMailBtn)
//    {
//        if (m_emailButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            TLog("clicked detail social email button");
//            m_isClickedMailBtn = false;
//            m_emailButton->setOpacity(255);
//        }
//    }else if (m_isClickedKakaoBtn)
//    {
//        if (m_kakaoButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            TLog("clicked detail social kakaotalk button");
//            m_isClickedKakaoBtn = false;
//            m_kakaoButton->setOpacity(255);
//        }
//    }else if (m_isClickedCloseBtn)
//    {
//        if (m_closeButton->getBoundingBox().containsPoint(m_window->convertToNodeSpace(location)))
//        {
//            TLog("clicked detail close button");
//            closeDetailWindow();
//            m_isClickedCloseBtn = false;
//            m_closeButton->setOpacity(255);
//        }
//    }
//}

/*--------------------------------- unused method ---------------------------------*/






























