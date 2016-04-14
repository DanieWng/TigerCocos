//
//  MoreAppsButton.cpp
//  TigerAPI
//
//  Created by Wang zhen on 4/5/16.
//
//

#include "MoreAppsButton.hpp"
#include <BaseAppConfig.hpp>
#include <TigerFunctions.h>

#define RIGHT_BUTTON_CONTENT_SIZE  cocos2d::Size(294, 393)
#define LEFT_BUTTON_CONTENT_SIZE cocos2d::Size(375, 267)

#define R_T_CLIPPING_MARGIN_LEFT    38
#define R_T_CLIPPING_WITDH          219
#define L_T_CLIPPING_MARGIN_BOTTOM  32
#define L_T_CLIPPING_HEIGHT         171

MoreAppsButton::MoreAppsButton()
{
    _moreText = nullptr;
    _icon = nullptr;
    _clippingNode = nullptr;
    
    _moveToFont = true;
}

MoreAppsButton::~MoreAppsButton()
{
}


MoreAppsButton* MoreAppsButton::create(MoreAppsButton::MoreAppsButtonType type)
{
    auto ret = new (std::nothrow)MoreAppsButton();
    
    if (ret && ret->initWithType(type))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
        ret = nullptr;
    }
    
    return ret;
}

void MoreAppsButton::onEnter()
{
    Layout::onEnter();
    
    this->runAction(RepeatForever::create(Sequence::create(CallFunc::create(LAMBDA_FUNCTION_START
                                                                            this->playEffectAnimationStart();
                                                                            LAMBDA_FUNCTION_END),
                                                           DelayTime::create(4.0f),
                                                           nullptr)));
}

bool MoreAppsButton::initWithType(MoreAppsButton::MoreAppsButtonType type)
{
    if (!Layout::init())
    {
        return false;
    }
    
    bool is_last_app = false;
    if (BaseAppConfig::getInstance()->getCurVersionData()._lastAppReleaseDate == APP_RELEASE_DATE)
    {
        is_last_app = true;
        TLog("MoreAppsButton::initWithType -- current app is last release app");
    }else
    {
        TLog("MoreAppsButton::initWithType -- has new app");
    }
    
    switch (type)
    {
        case MoreAppsButtonType::kRightTop:
            if (!initWithRightTop(is_last_app)) return false;
            break;
            
        case MoreAppsButtonType::kRightBottom:
            if (!initWithRightBottom(is_last_app)) return false;
            break;
            
        case MoreAppsButtonType::kLeftTop:
            if (!initWithLeftTop(is_last_app)) return false;
            break;
            
        default:
            break;
    }
    
    _type = type;
    
    return true;
}

bool MoreAppsButton::initWithRightTop(bool isLastApp)
{
    const float scene_scale = BaseAppConfig::getInstance()->getSceneScaleFactor();
    auto content_size = RIGHT_BUTTON_CONTENT_SIZE*(scene_scale/2.0f);
    this->setContentSize(content_size);
    
    auto bg = Sprite::create("btn_new/btn_r_top.png");
    IF_NULL_THEN_RETUEN(bg, false);
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(bg);
    
    _moreText = Sprite::create("btn_new/r_top_more_text.png");
    IF_NULL_THEN_RETUEN(_moreText, false);
    Tiger::setPosBaseRetina(_moreText,
                            Vec2(43, 369),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    this->addChild(_moreText);
    
    _clippingNode = ClippingNode::create();

    auto stencil = DrawNode::create();
    stencil->drawSolidRect(Vec2(0, 0),
                           Vec2(219, 196)*(scene_scale/2.0f),
                           Color4F::GREEN);
    Tiger::setPosBaseRetina(stencil,
                            Vec2(38, 110),
                            Vec2::ANCHOR_BOTTOM_LEFT,
                            scene_scale/2.0f,
                            false);
    _clippingNode->setStencil(stencil);
    
    _clippingNode->setInverted(false);
    
    _diagonalLine = Sprite::create("btn_new/l_top_diagonal_line.png");
    IF_NULL_THEN_RETUEN(_diagonalLine, false);
    
    const Size diagonal_content_size = _diagonalLine->getContentSize();
    float d_start_x = (R_T_CLIPPING_WITDH+R_T_CLIPPING_MARGIN_LEFT)*(scene_scale/2.0f) - diagonal_content_size.width;
    
    Tiger::setPosBaseRetina(_diagonalLine,
                            Vec2(d_start_x, 306),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(_diagonalLine);
    
    std::string icon_res = isLastApp==false?"btn_new/new_icon.png":"btn_new/second_icon.png";
    _icon = Sprite::create(icon_res);
    IF_NULL_THEN_RETUEN(_icon, false);
    
    const Size icon_content_size = _icon->getContentSize();
    float i_start_x = 0 - icon_content_size.width;
    
    Tiger::setPosBaseRetina(_icon,
                            Vec2(i_start_x, 294),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(_icon);
    
    auto light = Sprite::create("btn_new/r_top_side_light.png");
    IF_NULL_THEN_RETUEN(light, false);
    Tiger::setPosBaseRetina(light,
                            Vec2(38, 306),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(light);

    this->addChild(_clippingNode);
    
    _diagonalStartPos = _diagonalLine->getPosition();
    _iconStartPos = _icon->getPosition();
    _iconTargetPos = Vec2(61, 294)*(scene_scale/2.0f);
    
    return true;
}

bool MoreAppsButton::initWithRightBottom(bool isLastApp)
{
    const float scene_scale = BaseAppConfig::getInstance()->getSceneScaleFactor();
    auto content_size = RIGHT_BUTTON_CONTENT_SIZE*(scene_scale/2.0f);
    this->setContentSize(content_size);
    
    auto bg = Sprite::create("btn_new/btn_r_bottom.png");
    IF_NULL_THEN_RETUEN(bg, false);
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(bg);
    
    _moreText = Sprite::create("btn_new/more_text.png");
    IF_NULL_THEN_RETUEN(_moreText, false);
    Tiger::setPosBaseRetina(_moreText,
                            Vec2(43, 89),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    this->addChild(_moreText);
    
    _clippingNode = ClippingNode::create();

    auto stencil = DrawNode::create();
    stencil->drawSolidRect(Vec2(0, 0),
                           Vec2(219, 196)*(scene_scale/2.0f),
                           Color4F::GREEN);
    Tiger::setPosBaseRetina(stencil,
                            Vec2(38, 89),
                            Vec2::ANCHOR_BOTTOM_LEFT,
                            scene_scale/2.0f,
                            false);
    _clippingNode->setStencil(stencil);
    
    _clippingNode->setInverted(false);
    
    _diagonalLine = Sprite::create("btn_new/l_top_diagonal_line.png");
    IF_NULL_THEN_RETUEN(_diagonalLine, false);
    
    const Size diagonal_content_size = _diagonalLine->getContentSize();
    float d_start_x = (R_T_CLIPPING_WITDH+R_T_CLIPPING_MARGIN_LEFT)*(scene_scale/2.0f) - diagonal_content_size.width;
    
    Tiger::setPosBaseRetina(_diagonalLine,
                            Vec2(d_start_x, 285),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(_diagonalLine);
    
    std::string icon_res = isLastApp==false?"btn_new/new_icon.png":"btn_new/second_icon.png";
    _icon = Sprite::create(icon_res);
    IF_NULL_THEN_RETUEN(_icon, false);
    
    const Size icon_content_size = _icon->getContentSize();
    float i_start_x = 0 - icon_content_size.width;
    
    Tiger::setPosBaseRetina(_icon,
                            Vec2(i_start_x, 275),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(_icon);
    
    auto light = Sprite::create("btn_new/side_light.png");
    IF_NULL_THEN_RETUEN(light, false);
    Tiger::setPosBaseRetina(light,
                            Vec2(38, 285),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(light);
    
    this->addChild(_clippingNode);

    _diagonalStartPos = _diagonalLine->getPosition();
    _iconStartPos = _icon->getPosition();
    _iconTargetPos = Vec2(61, 275)*(scene_scale/2.0f);
    
    return true;
}

bool MoreAppsButton::initWithLeftTop(bool isLastApp)
{
    const float scene_scale = BaseAppConfig::getInstance()->getSceneScaleFactor();
    auto content_size = LEFT_BUTTON_CONTENT_SIZE*(scene_scale/2.0f);
    this->setContentSize(content_size);
    
    auto bg = Sprite::create("btn_new/btn_l_top.png");
    IF_NULL_THEN_RETUEN(bg, false);
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(bg);
    
    _moreText = Sprite::create("btn_new/l_top_more_text.png");
    IF_NULL_THEN_RETUEN(_moreText, false);
    Tiger::setPosBaseRetina(_moreText,
                            Vec2(88, 259),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    this->addChild(_moreText);
    
    _clippingNode = ClippingNode::create();
    
    auto stencil = Sprite::create("btn_new/l_top_side_light.png");
    IF_NULL_THEN_RETUEN(stencil, false);
    Tiger::setPosBaseRetina(stencil,
                            Vec2(79, 203),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    _clippingNode->setStencil(stencil);
    
    _clippingNode->setInverted(false);
    _clippingNode->setAlphaThreshold(1.0f);
    
    _diagonalLine = Sprite::create("btn_new/diagonal_line.png");
    IF_NULL_THEN_RETUEN(_diagonalLine, false);
    
    const Size diagonal_content_size = _diagonalLine->getContentSize();
    float d_start_y = L_T_CLIPPING_MARGIN_BOTTOM*(scene_scale/2.0f);
    
    Tiger::setPosBaseRetina(_diagonalLine,
                            Vec2(79, d_start_y),
                            Vec2::ANCHOR_BOTTOM_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(_diagonalLine);
    
    std::string icon_res = isLastApp==false?"btn_new/l_top_new_icon.png":"btn_new/l_top_second_icon.png";
    _icon = Sprite::create(icon_res);
    IF_NULL_THEN_RETUEN(_icon, false);
    
    const Size icon_content_size = _icon->getContentSize();
    float i_start_y = (L_T_CLIPPING_MARGIN_BOTTOM+L_T_CLIPPING_HEIGHT)*(scene_scale/2.0f);
    
    Tiger::setPosBaseRetina(_icon,
                            Vec2(107, i_start_y),
                            Vec2::ANCHOR_BOTTOM_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(_icon);
    
    auto light = Sprite::create("btn_new/l_top_side_light.png");
    IF_NULL_THEN_RETUEN(light, false);
    Tiger::setPosBaseRetina(light,
                            Vec2(79, 203),
                            Vec2::ANCHOR_TOP_LEFT,
                            scene_scale/2.0f);
    _clippingNode->addChild(light);

    
    this->addChild(_clippingNode);
    
    _diagonalStartPos = _diagonalLine->getPosition();
    _iconStartPos = _icon->getPosition();
    _iconTargetPos = Vec2(107, 199)*(scene_scale/2.0f);
    _iconTargetPos.y -= _icon->getContentSize().height;
    
    return true;
}

void MoreAppsButton::playEffectAnimationStart()
{
    const Size button_content_size = this->getContentSize();
    
    if (_moveToFont)
    {
        _moveToFont = false;
 
        if (_type == MoreAppsButtonType::kRightBottom ||
            _type == MoreAppsButtonType::kRightTop)
        {
            _diagonalLine->runAction(Sequence::create(Place::create(_diagonalStartPos),
                                                      MoveTo::create(2.0f, Vec2(button_content_size.width, _diagonalStartPos.y)),
                                                      nullptr));
            
            _icon->runAction(Sequence::create(MoveTo::create(1.0f, Vec2(button_content_size.width, _iconStartPos.y)),
                                              DelayTime::create(0.1f),
                                              Place::create(_iconStartPos),
                                              MoveTo::create(0.75f, _iconTargetPos),
                                              nullptr));
        }
        else if (_type == MoreAppsButtonType::kLeftTop)
        {
            const float scene_scale = BaseAppConfig::getInstance()->getSceneScaleFactor();
            
            float d_move_to_y = -_diagonalLine->getContentSize().height + L_T_CLIPPING_MARGIN_BOTTOM*(scene_scale/2.0f);
            _diagonalLine->runAction(Sequence::create(Place::create(_diagonalStartPos),
                                                      MoveTo::create(2.0f, Vec2(_diagonalStartPos.x, d_move_to_y)),
                                                      nullptr));
            
            float i_move_to_y = -_icon->getContentSize().height + L_T_CLIPPING_MARGIN_BOTTOM*(scene_scale/2.0f);
            _icon->runAction(Sequence::create(MoveTo::create(1.0f, Vec2(_iconStartPos.x, i_move_to_y)),
                                              DelayTime::create(0.1f),
                                              Place::create(_iconStartPos),
                                              MoveTo::create(0.75f, _iconTargetPos),
                                              nullptr));
        }
        
    }else
    {
        _moveToFont = true;
        
        if (_type == MoreAppsButtonType::kRightBottom ||
            _type == MoreAppsButtonType::kRightTop)
        {
            auto line_content_size = _diagonalLine->getContentSize();
            auto icon_content_size = _icon->getContentSize();
            
            _diagonalLine->runAction(Sequence::create(MoveTo::create(2.0f, Vec2(0-line_content_size.width, _diagonalStartPos.y)),
                                                      nullptr));
            
            _icon->runAction(Sequence::create(MoveTo::create(1.0f, Vec2(0-icon_content_size.width, _iconStartPos.y)),
                                              Place::create(Vec2(button_content_size.width, _iconStartPos.y)),
                                              DelayTime::create(0.1f),
                                              MoveTo::create(0.75f, _iconTargetPos),
                                              nullptr));
        }
        else if (_type == MoreAppsButtonType::kLeftTop)
        {
            const float scene_scale = BaseAppConfig::getInstance()->getSceneScaleFactor();
            
            _diagonalLine->runAction(Sequence::create(MoveTo::create(2.0f,
                                                                     Vec2(_diagonalStartPos.x,
                                                                          (L_T_CLIPPING_MARGIN_BOTTOM+L_T_CLIPPING_HEIGHT)*(scene_scale/2.0f))),
                                                      nullptr));
            
            _icon->runAction(Sequence::create(MoveTo::create(1.0f,
                                                             Vec2(_iconStartPos.x,
                                                                  (L_T_CLIPPING_MARGIN_BOTTOM+L_T_CLIPPING_HEIGHT)*(scene_scale/2.0f))),
                                              Place::create(Vec2(_iconStartPos.x,
                                                                 -_icon->getContentSize().height+L_T_CLIPPING_MARGIN_BOTTOM*(scene_scale/2.0f))),
                                              DelayTime::create(0.1f),
                                              MoveTo::create(0.75f, _iconTargetPos),
                                              nullptr));
        }
    }
    
    _moreText->runAction(Sequence::create(DelayTime::create(1.7f),
                                          FadeOut::create(0.15f),
                                          DelayTime::create(0.1f),
                                          FadeIn::create(0.15f),
                                          nullptr));
}















