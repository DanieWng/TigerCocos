//
//  TWebView.hpp
//  FactoryAppAirplane
//
//  Created by Wang zhen on 5/27/16.
//
//

#ifndef TWebView_hpp
#define TWebView_hpp

#include "TigerBaseLayer.hpp"
#include <ui/CocosGUI.h>

using namespace ui;

#define WEB_LOADING_NAME "web_loading"

class TWebView : public TigerBaseLayer
{
public:
    
    ~TWebView();
    
    virtual bool init() override;
    CREATE_FUNC(TWebView);
    
    void loadUrl(const std::string url);
    
private:
    
    void playLoadingAnimation();
    
private:
    
    cocos2d::experimental::ui::WebView *_webView;
    
    Sprite *_loading;
    
private:
    
    void loadFinishedEvent(experimental::ui::WebView *sender, const std::string &url);
    bool shouldStartLoading(experimental::ui::WebView *sender, const std::string &url);
};

#endif /* TWebView_hpp */
