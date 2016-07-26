//
//  TigerAutoPolygonSprite.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 4/27/16.
//
//

#ifndef TigerAutoPolygonSprite_hpp
#define TigerAutoPolygonSprite_hpp

#include "cocos2d.h"

USING_NS_CC;

class TigerAutoPolygonSprite : public Sprite
{
public:
    
    static TigerAutoPolygonSprite* create(const std::string& file, bool usePolygon=true);
    
    bool initWithFile(const std::string& file, bool usePolygon);
    
protected:
    
};

#endif /* TigerAutoPolygonSprite_hpp */
