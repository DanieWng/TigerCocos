//
//  NewsLetterPopupLayer.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 4/11/16.
//
//

#ifndef NewsLetterPopupLayer_hpp
#define NewsLetterPopupLayer_hpp

#include "../base/TigerBasePopupLayer.hpp"
#include "../network/TigerNewsLetter.hpp"
#include <ui/CocosGUI.h>

using namespace cocos2d::ui;

class NewsLetterPopupLayer : public TigerBasePopupLayer,
                             public EditBoxDelegate,
                             public TigerNewsLetterHttpDelegate
{
public:
    
    NewsLetterPopupLayer();
    ~NewsLetterPopupLayer();
    
    virtual void onEnter() override;
    
    virtual bool init() override;
    CREATE_FUNC(NewsLetterPopupLayer);
    
private:
    
    void buttonTouchEvent(Ref *ref, Widget::TouchEventType type);
    
    void addEmailIntoNewsLetter();
    
    virtual void editBoxEditingDidBegin(EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(EditBox* editBox) override;
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(EditBox* editBox) override;
    
    virtual void putNewsLetterHttpStatusCode(const int code) override;
    
private:
    
    enum UIButtonTag
    {
        kOk,
        kCancel
    };
    
    EditBox *_email;
    EditBox *_firstName;
    EditBox *_lastName;
    
};

#endif /* NewsLetterPopupLayer_hpp */
