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

struct tBeizer6PointConfig
{
    Vec2 _point0;
    Vec2 _point1;
    Vec2 _point2;
    Vec2 _point3;
    Vec2 _point4;
    Vec2 _point5;
};

static inline float customEaseOut(float time)
{
    if (time <= 0.5)
    {
        time = (-powf(2, -10 * time / 1) + 1);
    }
    
    return time;
}

// 三阶贝赛尔曲线的计算公式
// Bezier cubic formula:
//    ((1 - t) + t)3 = 1
// Expands to
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1
static inline float bezierat3( float a, float b, float c, float d, float t )
{
    return (powf(1-t,3) * a +
            3.0f*t*(powf(1-t,2))*b +
            3.0f*powf(t,2)*(1-t)*c +
            powf(t,3)*d );
}

// 二次贝赛尔计算公式
static inline float bezierat2( float a, float b, float d, float t )
{
    return (powf(1-t,2) * a +
            2.0f*t*(1-t)*b +
            powf(t,2)*d );
}

static inline float bezierat5(float a, float b, float c, float d, float e, float f, float t)
{
    return powf(1-t, 5)*a + 5*t*pow(1-t, 4)*b + 10*pow(t, 2)*pow(1-t, 3)*c + 10*pow(t, 3)*pow(1-t, 2)*d + 5*pow(t, 4)*pow(1-t, 1)*e + pow(t, 5)*f;
}

class TArrowPathBezier : public ActionInterval
{
public:
    
    TArrowPathBezier();
    
    static TArrowPathBezier* create(float t, const tBeizer6PointConfig& config);
    
    bool initWithDuration(float t, const tBeizer6PointConfig& config);
    
    virtual void update(float time) override;
    
    virtual void startWithTarget(Node *target) override;
    
    void setAcceleration(float a);
    
private:
    
    Vec2 _startPosition;
    Vec2 _previousPosition;
    
    tBeizer6PointConfig _config;
    
    float _accelerationDif;
};

#endif /* TArrowPathBezier_hpp */
