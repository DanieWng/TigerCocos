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

#include "TigerBaseLayer.hpp"

typedef std::function<void(float)> fAsyncLoadPercentDelegate;

class TigerBaseScene : public TigerBaseLayer
{
public:
    
    TigerBaseScene();
    virtual ~TigerBaseScene();
    
    virtual void onEnter() override;
    virtual bool init() override;
    
    void setSceneLoadPercentDelegate(const fAsyncLoadPercentDelegate d);
    
    virtual void initScene()=0;
    
protected:
    
    void setKeyboardListenerEnable(bool enable);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) override;
    
    void showExitWarnningMsg();
    
    virtual void doBackKeyClickEvent(){};
    
protected:
    
    fAsyncLoadPercentDelegate _loadPercentdelegate;
    int _asyncCompletedCount;
    
private:
    
    virtual void asyncLoadResource() {};
    virtual void asyncImageCallback(Texture2D *texture) {};
    virtual void addBG(){};
    
    virtual void preloadEffect() {};

};

#endif /* TigerBaseScene_hpp */








