//
//  TigerBaseButton.hpp
//  Tiger
//
//  添加点击后的状态功能，并重新设置贴图
//  Created by Wang zhen on 1/21/16.
//
//

#ifndef TigerBaseButton_hpp
#define TigerBaseButton_hpp

#include <ui/CocosGUI.h>

using namespace cocos2d::ui;

class TigerBaseButton : public Button
{
public:
    
    TigerBaseButton();
    virtual ~TigerBaseButton();
    
    static TigerBaseButton* create(const std::string& normal,
                                   const std::string& select="",
                                   const std::string& disable="",
                                   Widget::TextureResType type=Widget::TextureResType::LOCAL);
    
    virtual bool initWithFile(const std::string& normal,
                              const std::string& select,
                              const std::string& disable,
                              Widget::TextureResType type);
    
    void setIsSelected(bool isSelected);
    bool isSelected() { return _isSelected; };
    
    void swapNormalAndSelectTextures();
    
    virtual void loadTextures(const std::string& normal,
                         const std::string& select,
                         const std::string& disable,
                         Widget::TextureResType type);
    
protected:
    
    std::string _normalFile, _selectFile, _disableFile;
    
    bool _isSelected;
    
    Widget::TextureResType _resType;
};

#endif /* TigerBaseButton_hpp */











