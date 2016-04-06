//
//  TigerPolygonButton.hpp
//  NumbersLadyDemo
//
//  Created by Wang zhen on 3/30/16.
//
//

#ifndef TigerPolygonButton_hpp
#define TigerPolygonButton_hpp

#include "TigerBaseButton.hpp"
#include "PhysicsShapeCache.hpp"

USING_NS_CC;

class TigerPolygonButton : public TigerBaseButton
{
public:
    
    TigerPolygonButton();
    ~TigerPolygonButton();
    
    static TigerPolygonButton* create(const std::string& normal,
                                      const std::string& select="",
                                      const std::string& disable="",
                                      Widget::TextureResType type=Widget::TextureResType::LOCAL);
    
    virtual bool initWithFile(const std::string& normal,
                              const std::string& select,
                              const std::string& disable,
                              Widget::TextureResType type) override;
    
    /**
     添加刚体
     @params shape 对应plist文件内的图片名称
     @params fixedPriority   对应刚体多边形的优先值，当有多个刚体重叠时，优先值大的物体
                             优先拦截触摸事件
     */
    void addCollisionDetectionShape(const std::string& shape,
                                    int fixedPriority,
                                    int bodyTag);
    
    void setTouchListenerEnable(bool enable);
    
    typedef std::function<void(Touch*)> fButtonTouchDelegate;
    void setButtonTouchDelegate(const fButtonTouchDelegate d);
    
private:
    
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    
private:
    
    int _btnPriority;
    int _btnTag;
    
    fButtonTouchDelegate _touchDelegate;
};

#endif /* TigerPolygonButton_hpp */










