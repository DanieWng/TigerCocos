//
//  TigerBaseScene.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 1/15/16.
//
//

#include "TigerBaseScene.hpp"


TigerBaseScene::TigerBaseScene()
{

}

TigerBaseScene::~TigerBaseScene()
{
    _loadPercentdelegate = nullptr;
}

bool TigerBaseScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _loadPercentdelegate = nullptr;
    
    return true;
}

void TigerBaseScene::setSceneLoadPercentDelegate(const fAsyncLoadPercentDelegate d)
{
    _loadPercentdelegate = d;
    initScene();
}
