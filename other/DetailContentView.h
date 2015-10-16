//
//  DetailContentView.h
//  AVAD
//
//  For DetailWindow
//  If the content's size bigger than visiable size, then make its can be scrolling.
//  If the content's size small than visiable size, then just use lable and add to layout.
//
//  Created by Wang zhen on 3/20/15.
//
//

#ifndef __AVAD__DetailContentView__
#define __AVAD__DetailContentView__

#include "AppConfig.h"
#include "../cocos2d/cocos/ui/CocosGUI.h"
#include "../cocos2d/extensions/cocos-ext.h"
#include "DetailDataModel.h"

enum DetailContentViewDirection
{
    Horizontal,
    Vertical,
    None
};

using namespace cocos2d::ui;
USING_NS_CC_EXT;

class DetailContentView : public Node
{
public:
    DetailContentView();
    ~DetailContentView();
    
    virtual bool init();
    CREATE_FUNC(DetailContentView);
    
    CC_SYNTHESIZE(Size, m_viewSize, ContentViewSize);
    CC_SYNTHESIZE(bool, m_touchEnable, Touch);
    CC_SYNTHESIZE(int,  m_currentPage, CurrentPage);
    CC_SYNTHESIZE(bool, m_isPageChanged, IsPageChanged);
    
    void initContentView(std::vector<DetailDataModel>* pContentData);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    DetailDataModel getCurrentDataModel();
    
    
private:
    
    const float CONTENT_MARGIN = 30.0f;
    const float LIMIT_SCROLL_LENGTH = 60;
    
    cocos2d::extension::ScrollView*     m_contentScrollView;
    cocos2d::extension::ScrollView*     m_labelScrollView;
    Vector<cocos2d::extension::ScrollView*> m_detailScroll_v;
    
    cocos2d::ui::Scale9Sprite*          m_labelSVBar;
    
    int m_totalPageCount;
    Vec2 m_firstPoint;
    Vec2 m_offset;
    Vec2 m_totalOffset;
    Vec2 m_labelSVOffset;

    bool  m_horizontal;
    bool  m_verticalEnable;
    float m_labelHeight;
    float m_labelSVBarContentHeight;
    
    void initListener();
    
    void adjustScrollView(float distance);
    void adjustLabelScrollView(float distance);
    void setLabelScrollViewBarContentOffset(float deltaY);
    void hideScrollViewBar();
    void scaleLabelScrollViewBar(float posY);
    
    std::vector<DetailDataModel> m_detailDatas_v;
    
};

#endif /* defined(__AVAD__DetailContentView__) */
