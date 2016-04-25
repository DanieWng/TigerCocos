//
//  NewsLetterPopupLayer.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 4/11/16.
//
//

#include "NewsLetterPopupLayer.hpp"
#include <AppConfig.hpp>
#include <TigerFunctions.h>

#define INPUT_TEXT_COLOR Color3B(80, 80, 80)
#define INPUT_TEXT_SIZE  32 // base on retina

NewsLetterPopupLayer::NewsLetterPopupLayer()
{
}

NewsLetterPopupLayer::~NewsLetterPopupLayer()
{
}

void NewsLetterPopupLayer::onEnter()
{
    TigerBasePopupLayer::onEnter();
    
}

bool NewsLetterPopupLayer::init()
{
    if (!TigerBasePopupLayer::init())
    {
        return false;
    }
    
    const Size visible_size = Director::getInstance()->getVisibleSize();
    const float scene_scale = AppConfig::getInstance()->getSceneScaleFactor();
    
    addShadowLayer(visible_size);
    
    auto bg = Sprite::create("common/newsletter/newsletter_pop_bg.png");
    Tiger::setPosBaseRetina(bg,
                            Vec2(514, 361),
                            Vec2::ANCHOR_BOTTOM_LEFT,
                            scene_scale/2.0f);
    this->addChild(bg);
    
    auto btn_cancel = Tiger::createButton("common/newsletter/newsletter_pop_btn_cancel.png",
                                          "common/newsletter/newsletter_pop_btn_cancel.png",
                                          "common/newsletter/newsletter_pop_btn_cancel.png",
                                          CC_CALLBACK_2(NewsLetterPopupLayer::buttonTouchEvent, this),
                                          Widget::TextureResType::LOCAL);
    Tiger::setPosBaseRetina(btn_cancel,
                            Vec2(104, 70),
                            Vec2::ANCHOR_BOTTOM_LEFT,
                            scene_scale/2.0f,
                            false);
    btn_cancel->setTag(UIButtonTag::kCancel);
    bg->addChild(btn_cancel);
    
    auto btn_ok = Tiger::createButton("common/newsletter/newsletter_pop_btn_ok.png",
                                      "common/newsletter/newsletter_pop_btn_ok.png",
                                      "common/newsletter/newsletter_pop_btn_ok.png",
                                      CC_CALLBACK_2(NewsLetterPopupLayer::buttonTouchEvent, this),
                                      Widget::TextureResType::LOCAL);
    Tiger::setPosBaseRetina(btn_ok,
                            Vec2(535, 70),
                            Vec2::ANCHOR_BOTTOM_LEFT,
                            scene_scale/2.0f,
                            false);
    btn_ok->setTag(UIButtonTag::kOk);
    bg->addChild(btn_ok);
    
    _email = EditBox::create(Size(520, 60)*(scene_scale/2.0f), "common/orange_edit.png");
    Tiger::setPosBaseRetina(_email,
                            Vec2(600, 633),
                            Vec2::ANCHOR_MIDDLE,
                            scene_scale/2.0f,
                            false);
    _email->setFontSize(INPUT_TEXT_SIZE*(scene_scale/2.0f));
    _email->setFontColor(INPUT_TEXT_COLOR);
    _email->setDelegate(this);
    _email->setReturnType(EditBox::KeyboardReturnType::DONE);
    _email->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    bg->addChild(_email);
    
    _firstName = EditBox::create(Size(422, 60)*(scene_scale/2.0f), "common/orange_edit.png");
    Tiger::setPosBaseRetina(_firstName,
                            Vec2(651, 488),
                            Vec2::ANCHOR_MIDDLE,
                            scene_scale/2.0f,
                            false);
    _firstName->setFontSize(INPUT_TEXT_SIZE*(scene_scale/2.0f));
    _firstName->setFontColor(INPUT_TEXT_COLOR);
    _firstName->setDelegate(this);
    _firstName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _firstName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    bg->addChild(_firstName);
    
    _lastName = EditBox::create(Size(422, 60)*(scene_scale/2.0f), "common/orange_edit.png");
    Tiger::setPosBaseRetina(_lastName,
                            Vec2(651, 344),
                            Vec2::ANCHOR_MIDDLE,
                            scene_scale/2.0f,
                            false);
    _lastName->setFontSize(INPUT_TEXT_SIZE*(scene_scale/2.0f));
    _lastName->setFontColor(INPUT_TEXT_COLOR);
    _lastName->setDelegate(this);
    _lastName->setReturnType(EditBox::KeyboardReturnType::DONE);
    _lastName->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    bg->addChild(_lastName);
    
    _email->setOpacity(0);
    _firstName->setOpacity(0);
    _lastName->setOpacity(0);
    
    return true;
}

void NewsLetterPopupLayer::buttonTouchEvent(cocos2d::Ref *ref, Widget::TouchEventType type)
{
    auto btn = static_cast<Button*>(ref);
    auto tag = btn->getTag();
    
    switch (type)
    {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            btn->getVirtualRenderer()->setColor(Color3B(122, 122, 122));
            break;
            
        case cocos2d::ui::Widget::TouchEventType::ENDED:
        {
            switch (tag)
            {
                case kOk:
                    addEmailIntoNewsLetter();
                    break;
                    
                case kCancel:
                    this->hide();
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
}

void NewsLetterPopupLayer::addEmailIntoNewsLetter()
{
    auto member_data = Tiger::NewsLetterMemberData();
    
    member_data._mail = _email->getText();
    member_data._mailMD5 = Tiger::getMD5(member_data._mail);
    member_data._firstName = _firstName->getText();
    member_data._lastName = _lastName->getText();
    
    auto newsletter = Tiger::TigerNewsLetter::getInstance();
    newsletter->setDelegate(this);
    newsletter->addSubscriberToListID(MAILCHIMP_DATA_CENTER_CODE, YFACTORYAPPS_LIST_ID, member_data, true);
}

void NewsLetterPopupLayer::putNewsLetterHttpStatusCode(const int code)
{
    if (code == 200)
    {
        this->hide();
    }
    else if (code == 500)
    {
        MessageBox("Invalid Email Address", "Error");
    }
    
    Tiger::TigerNewsLetter::getInstance()->destoryInstance();
}

void NewsLetterPopupLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox)
{
}

void NewsLetterPopupLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox *editBox)
{
}

void NewsLetterPopupLayer::editBoxTextChanged(cocos2d::ui::EditBox *editBox, const std::string &text)
{
}

void NewsLetterPopupLayer::editBoxReturn(cocos2d::ui::EditBox *editBox)
{
}









