//
//  TigerBaseButton.cpp
//  Tiger
//
//  Created by Wang zhen on 1/21/16.
//
//

#include "TigerBaseButton.hpp"


TigerBaseButton::TigerBaseButton():
_normalFile(""),
_selectFile(""),
_disableFile(""),
_isSelected(false),
_resType(Widget::TextureResType::LOCAL)
{
    
}

TigerBaseButton::~TigerBaseButton()
{
    
}

TigerBaseButton* TigerBaseButton::create(const std::string& normal,
                                         const std::string& select,
                                         const std::string& disable,
                                         Widget::TextureResType type)
{
    auto ret = new (std::nothrow)TigerBaseButton();
    
    if (ret && ret->initWithFile(normal,
                                 select,
                                 disable,
                                 type))
    {
        ret->autorelease();
    }else
    {
        CC_SAFE_DELETE(ret);
        ret = nullptr;
    }
    
    return ret;
}

bool TigerBaseButton::initWithFile(const std::string &normal, const std::string &select, const std::string &disable, Widget::TextureResType type)
{
    if (!Button::init(normal, select, disable, type))
    {
        return false;
    }
    
    _normalFile = normal;
    _selectFile = select;
    _disableFile = disable;
    
    _resType = type;
    
    return true;
}

void TigerBaseButton::setIsSelected(bool isSelected)
{
    if (isSelected)
    {
        Button::loadTextures(_selectFile, _normalFile, _disableFile, _resType);
    }else
    {
        Button::loadTextures(_normalFile, _selectFile, _disableFile, _resType);
    }
    
    _isSelected = isSelected;
}


void TigerBaseButton::swapNormalAndSelectTextures()
{
    Button::loadTextures(_selectFile, _normalFile, _disableFile, _resType);

    auto temp = _normalFile;
    _normalFile = _selectFile;
    _selectFile = temp;
}

void TigerBaseButton::loadTextures(const std::string& normal,
                                   const std::string& select,
                                   const std::string& disable,
                                   Widget::TextureResType type)
{
    Button::loadTextures(normal, select, disable, type);
    
    _normalFile = normal;
    _selectFile = select;
    _disableFile = disable;
    
    _resType = type;
}






