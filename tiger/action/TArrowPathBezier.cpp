//
//  TArrowPathBezier.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 3/17/16.
//
//

#include "TArrowPathBezier.hpp"

TArrowPathBezier::TArrowPathBezier()
{
    _accelerationDif = 0.0f;
}

TArrowPathBezier* TArrowPathBezier::create(float t, const tBeizer6PointConfig &config)
{
    TArrowPathBezier *pBezierBy = new (std::nothrow) TArrowPathBezier();
    pBezierBy->initWithDuration(t, config);
    pBezierBy->autorelease();
    
    return pBezierBy;
}

bool TArrowPathBezier::initWithDuration(float t, const tBeizer6PointConfig &config)
{
    bool b_ret = false;
    
    if (ActionInterval::initWithDuration(t))
    {
        _config = config;
        return true;
    }
    
    return b_ret;
}

void TArrowPathBezier::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = target->getPosition();
}

void TArrowPathBezier::setAcceleration(float a)
{
    if (a > 300.0f)
    {
        a = 300.0f;
    }
    _accelerationDif = a/1000.0f;
}

void TArrowPathBezier::update(float time)
{
    if (_target)
    {
//        if (_accelerationDif > 0.0f)
//        {
//            float t = (_accelerationDif*_accelerationDif*_accelerationDif)*2;
//            
//            time += t;
//            
//            /*
//             次方基数越大，加速时间越长
//             */
//            _accelerationDif -= (powf(time, 7));
//            
//            CCLOG("TArrowPathBezier::update - accele [%f]", t);
//        }
        
        CCLOG("TArrowPathBezier::update - %f", time);
        
        float xa = _config._point0.x;
        float xb = _config._point1.x;
        float xc = _config._point2.x;
        float xd = _config._point3.x;
        float xe = _config._point4.x;
        float xf = _config._point5.x;
        
        float ya = _config._point0.y;
        float yb = _config._point1.y;
        float yc = _config._point2.y;
        float yd = _config._point3.y;
        float ye = _config._point4.y;
        float yf = _config._point5.y;
        
        float x = bezierat5(xa, xb, xc, xd, xe, xf, time);
        float y = bezierat5(ya, yb, yc, yd, ye, yf, time);
        
#if CC_ENABLE_STACKABLE_ACTIONS
        
        Vec2 currentPos = _target->getPosition();
        Vec2 diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;
        
        Vec2 newPos = _startPosition + Vec2(x,y);
        _target->setPosition(newPos);
        
        _previousPosition = newPos;
        
        if (0 != time) {
            Vec2 vector = currentPos - newPos;
            float radians = vector.getAngle();
            float degrees = CC_RADIANS_TO_DEGREES(-1 * radians);
            
            // 飞机因为贴图原因，需要事先旋转90角度
            _target->setRotation(degrees - 180 + 90);
        }
        
#else
        _target->setPosition( _startPosition + Vec2(x,y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
        
    }
}







