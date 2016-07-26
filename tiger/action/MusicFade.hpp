//
//  MusicFade.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 5/26/16.
//
//

#ifndef MusicFade_hpp
#define MusicFade_hpp

#include "cocos2d.h"

USING_NS_CC;

class MusicFade : public ActionInterval
{
public:
    
    MusicFade();
    ~MusicFade();
    
    static MusicFade* create(float d, float volume, bool isFadeOut);
    bool initWithDuration(float d, float volume, bool isFadeOut);
    
    virtual void update(float dt) override;
    virtual void startWithTarget(cocos2d::Node *pTarget) override;
    
protected:
    
    float _targetVolume;
    float _originalVolume;
    
    bool  _isFadeOut;
};

#endif /* MusicFade_hpp */
