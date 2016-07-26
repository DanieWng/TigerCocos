//
//  TigerBaseLayer.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 5/25/16.
//
//

#ifndef TigerBaseLayer_hpp
#define TigerBaseLayer_hpp

#include "cocos2d.h"
#include <SimpleAudioEngine.h>

USING_NS_CC;
using namespace CocosDenshion;

class TigerBaseLayer : public Layer
{
public:
    
    virtual void onEnter() override;
    
    virtual bool init() override;
    
    const unsigned int playEffect(bool isEfxEnable, const std::string file, bool isLoop=false);
    void playBGM(bool isBGMEnable, const std::string file, bool isLoop=false);
    
protected:
    
};

#endif /* TigerBaseLayer_hpp */
