//
//  TArrowPathBezier.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 3/17/16.
//
//

#ifndef TArrowPathBezier_hpp
#define TArrowPathBezier_hpp

#include <stdio.h>

#include "cocos2d.h"

USING_NS_CC;

class TArrowPathBezier : public BezierBy
{
public:
    
    TArrowPathBezier();
    
    static TArrowPathBezier* create(float t, const ccBezierConfig& config);
    
    bool initWithDuration(float t, const ccBezierConfig& config);
    
    virtual void update(float time);
    
    // get direction
    Vec2 getDirection();
    
private:
    
    Vec2 _dir;
};

#endif /* TArrowPathBezier_hpp */
