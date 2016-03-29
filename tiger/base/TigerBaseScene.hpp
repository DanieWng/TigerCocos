//
//  TigerBaseScene.hpp
//  FactoryAppAirplane
//
//  添加了可以返回加载进度的delegate代理
//  Created by Wang zhen on 1/15/16.
//
//

#ifndef TigerBaseScene_hpp
#define TigerBaseScene_hpp

#include "cocos2d.h"

USING_NS_CC;

typedef std::function<void(float)> fAsyncLoadPercentDelegate;

class TigerBaseScene : public Layer
{
public:
    
    TigerBaseScene();
    virtual ~TigerBaseScene();
    
    virtual bool init() override;
    
    void setSceneLoadPercentDelegate(const fAsyncLoadPercentDelegate d);
    
    virtual void initScene()=0;
    
protected:
    
    fAsyncLoadPercentDelegate _loadPercentdelegate;
    
private:
    
    virtual void addBG(){};
    
};

#endif /* TigerBaseScene_hpp */
