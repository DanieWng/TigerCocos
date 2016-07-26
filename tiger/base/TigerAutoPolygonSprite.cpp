//
//  TigerAutoPolygonSprite.cpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 4/27/16.
//
//

#include "TigerAutoPolygonSprite.hpp"


TigerAutoPolygonSprite* TigerAutoPolygonSprite::create(const std::string &file, bool usePolygon)
{
    auto ret = new (std::nothrow)TigerAutoPolygonSprite();
    
    if (ret && ret->initWithFile(file, usePolygon))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
        ret = nullptr;
    }
    
    return ret;
}

bool TigerAutoPolygonSprite::initWithFile(const std::string &file, bool usePolygon)
{
    if (usePolygon)
    {
        auto image = new Image();
        image->initWithImageFile(file);
        
        if (image->getRenderFormat() == Texture2D::PixelFormat::RGBA8888)
        {
            auto polygon_info = AutoPolygon::generatePolygon(file);
            
            if (!Sprite::initWithPolygon(polygon_info))
            {
                return false;
            }
        }
        else
        {
            if (!Sprite::initWithFile(file))
            {
                return false;
            }
        }
        
        CC_SAFE_DELETE(image);
    }
    else
    {
        if (!Sprite::initWithFile(file))
        {
            return false;
        }
    }
    
    return true;
}



















