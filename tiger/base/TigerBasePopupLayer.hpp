//
//  TigerBasePopupLayer.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 1/14/16.
//
//

#ifndef TigerBasePopupLayer_hpp
#define TigerBasePopupLayer_hpp

#include <cocos2d.h>
#include "../TigerEnum.h"

USING_NS_CC;
USING_NS_T;

typedef std::function<void(Ref*, LayerStatus)> fLayerStatusDelegate;

class TigerBasePopupLayer : public Layer
{
public:
    
    TigerBasePopupLayer();
    virtual ~TigerBasePopupLayer();
    
    virtual bool init() override;
    
    void setPopupLayerStatusDelegate(const fLayerStatusDelegate d);
    
    virtual void setTouchListenerEnable(bool enable);
    
    void addShadowLayer(const Size size);
    Node* getShadowLayer();
    
    virtual void show();
    virtual void hide();
    
protected:
    
    fLayerStatusDelegate _layerStatusDelegate;
    
    EventListenerTouchOneByOne *_touchListener;
    
};

#endif /* TigerBasePopupLayer_hpp */
