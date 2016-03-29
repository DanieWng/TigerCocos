//
//  TArrowPathBezier.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 3/17/16.
//
//

#include "TArrowPathBezier.hpp"


// 原来贝赛尔曲线的计算公式
// Bezier cubic formula:
//    ((1 - t) + t)3 = 1
// Expands to
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1
static inline float bezierat( float a, float b, float c, float d, float t )
{
    return (powf(1-t,3) * a +
            3.0f*t*(powf(1-t,2))*b +
            3.0f*powf(t,2)*(1-t)*c +
            powf(t,3)*d );
}

// 二次贝赛尔计算公式
static inline float bezierat1( float a, float b, float d, float t )
{
    return (powf(1-t,2) * a +
            2.0f*t*(1-t)*b +
            powf(t,2)*d );
}

TArrowPathBezier::TArrowPathBezier():
_dir(Vec2::ZERO)
{
}

TArrowPathBezier* TArrowPathBezier::create(float t, const ccBezierConfig &config)
{
    TArrowPathBezier *pBezierBy = new (std::nothrow) TArrowPathBezier();
    pBezierBy->initWithDuration(t, config);
    pBezierBy->autorelease();
    
    return pBezierBy;
}

bool TArrowPathBezier::initWithDuration(float t, const ccBezierConfig &config)
{
    bool b_ret = false;
    
    if (BezierBy::initWithDuration(t, config))
    {
        _config = config;
        return true;
    }
    
    return b_ret;
}

void TArrowPathBezier::update(float time)
{
    auto pre_pos = _previousPosition;
    
    BezierBy::update(time);
    
    if (0 != time) {
        Vec2 vector = pre_pos - _target->getPosition();
        float radians = vector.getAngle();
        float degrees = CC_RADIANS_TO_DEGREES(-1 * radians);
        
        // 飞机因为贴图原因，需要事先旋转90角度
        _target->setRotation(degrees - 180 + 90);
    }
}

Vec2 TArrowPathBezier::getDirection()
{
    return _dir;
}







