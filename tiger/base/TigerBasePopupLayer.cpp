//
//  TigerBasePopupLayer.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 1/14/16.
//
//

#include "TigerBasePopupLayer.hpp"

#define SHADOW_LAYER_TAG 7878

TigerBasePopupLayer::TigerBasePopupLayer()
{
    _layerStatusDelegate = nullptr;
    _touchListener = nullptr;
}

TigerBasePopupLayer::~TigerBasePopupLayer()
{
}

bool TigerBasePopupLayer::init()
{
    if (!TigerBaseLayer::init())
    {
        return false;
    }
    
    return true;
}

void TigerBasePopupLayer::addShadowLayer(const cocos2d::Size size)
{
    auto shadow_layer = LayerColor::create(Color4B(0, 0, 0, 178), size.width, size.height);
    shadow_layer->setTag(SHADOW_LAYER_TAG);
    this->addChild(shadow_layer, -1);
}

Node* TigerBasePopupLayer::getShadowLayer()
{
    return this->getChildByTag(SHADOW_LAYER_TAG);
}

void TigerBasePopupLayer::setPopupLayerStatusDelegate(const fLayerStatusDelegate d)
{
    _layerStatusDelegate = d;
}

void TigerBasePopupLayer::setTouchListenerEnable(bool enable)
{
    if (enable)
    {
        if (_touchListener)
        {
            _eventDispatcher->removeEventListener(_touchListener);
            _touchListener = nullptr;
        }
        
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->setSwallowTouches(true);
        
        _touchListener->onTouchBegan = [&](Touch *touch, Event *event)->bool{
            return true;
        };
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
        
    }else
    {
        if (_touchListener)
        {
            _eventDispatcher->removeEventListener(_touchListener);
            _touchListener = nullptr;
        }
    }
}

void TigerBasePopupLayer::show()
{
    this->setVisible(true);
    setTouchListenerEnable(true);
    
    if (_layerStatusDelegate)
    {
        _layerStatusDelegate(this, LayerStatus::kShow);
    }
}

void TigerBasePopupLayer::hide()
{
    this->setVisible(false);
    setTouchListenerEnable(false);
    
    if (_layerStatusDelegate)
    {
        _layerStatusDelegate(this, LayerStatus::kHide);
    }
}




















