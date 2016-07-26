//
//  MusicFade.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 5/26/16.
//
//

#include "MusicFade.hpp"
#include <SimpleAudioEngine.h>

using namespace CocosDenshion;

MusicFade::MusicFade()
{
    _targetVolume = 0.0f;
    
    _isFadeOut = true;
}

MusicFade::~MusicFade()
{
}

MusicFade* MusicFade::create(float d, float volume, bool isFadeOut)
{
    auto ret = new (std::nothrow)MusicFade();
    
    if (ret && ret->initWithDuration(d, volume, isFadeOut))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

bool MusicFade::initWithDuration(float d, float volume, bool isFadeOut)
{
    if (!ActionInterval::initWithDuration(d))
    {
        return false;
    }
    
    _targetVolume = volume;
    
    _isFadeOut = isFadeOut;
    
    return true;
}

void MusicFade::startWithTarget(cocos2d::Node *pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    _originalVolume = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
}

void MusicFade::update(float dt)
{
    if (_target)
    {
        float cur_volume = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
        
        cur_volume = _originalVolume + dt*(_targetVolume-_originalVolume);
        
        if (cur_volume < 0.0f)
        {
            cur_volume = 0.0f;
        }
        else if (cur_volume > 1.0f)
        {
            cur_volume = 1.0f;
        }
        
//        CCLOG("MusicFade::update -- volume : %f", cur_volume);
        
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(cur_volume);
    }
}

















