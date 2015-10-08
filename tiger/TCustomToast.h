//
//  CustomToast.h
//  TheSolarSystem
//
//  Created by Wang zhen on 11/25/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#ifndef __TheSolarSystem__CustomToast__
#define __TheSolarSystem__CustomToast__

#include <stdio.h>

#include "cocos2d.h"
#include "TigerMacros.h"

NS_T_BEGIN

class CustomToast : public cocos2d::Node
{
public:
    
    CustomToast();
    ~CustomToast();
    
    virtual bool init();
    virtual bool initWithMsg(const std::string& msg);
    
    static CustomToast* create();
    static CustomToast* create(const std::string& msg);
    
    void setToastMsg(const std::string& msg);
    void show();
    
private:
    
    void removeSelf(float dt);
    
    cocos2d::LabelTTF   *_toastMsg;
    cocos2d::LayerColor *_bgLayer;
    cocos2d::Size       _contentSize;
    
};

NS_T_END

#endif /* defined(__TheSolarSystem__CustomToast__) */
