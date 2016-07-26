//
//  TigerBaseLayer.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 5/25/16.
//
//

#include "TigerBaseLayer.hpp"
#include "MusicFade.hpp"

void TigerBaseLayer::onEnter()
{
    Layer::onEnter();
}

bool TigerBaseLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    return true;
}

const unsigned int TigerBaseLayer::playEffect(bool isEfxEnable, const std::string file, bool isLoop)
{
    unsigned int id = 0;
    
    if (isEfxEnable)
    {
        id = SimpleAudioEngine::getInstance()->playEffect(file.c_str(), isLoop);
    }
    
    return id;
}

void TigerBaseLayer::playBGM(bool isBGMEnable, const std::string file, bool isLoop)
{
    CCLOG("\nTigerBaseLayer::playBGM [%s] - [%s]\n", isBGMEnable?"true":"false", file.c_str());
    
    if (isBGMEnable)
    {
        auto play_bgm = [&, file, isLoop](){
            SimpleAudioEngine::getInstance()->playBackgroundMusic(file.c_str(), isLoop);
        };
        
        if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        {
            this->runAction(Sequence::create(MusicFade::create(0.5f, 0.0f, true),
                                             CallFunc::create(play_bgm),
                                             MusicFade::create(0.5f, 1.0f, false),
                                             nullptr));
        }
        else
        {
            SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
            this->runAction(Sequence::create(CallFunc::create(play_bgm),
                                             MusicFade::create(0.5f, 1.0f, false),
                                             nullptr));
        }
        
    }
}







