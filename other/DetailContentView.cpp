//
//  DetailContentView.cpp
//  AVAD
//
//  Created by Wang zhen on 3/20/15.
//
//

#include "DetailContentView.h"
#include "../cocos2d/cocos/ui/UIEditBox/UIEditBox.h"

using namespace TigerFunctions;

DetailContentView::DetailContentView():
m_contentScrollView(nullptr),
m_offset(Vec2::ZERO),
m_totalOffset(Vec2::ZERO),
m_horizontal(true),
m_verticalEnable(false),
m_currentPage(0),
m_isPageChanged(false)
{
    
}

DetailContentView::~DetailContentView()
{

}

bool DetailContentView::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    return true;
}

void DetailContentView::initContentView(std::vector<DetailDataModel> *pContentData)
{
    m_contentScrollView = cocos2d::extension::ScrollView::create(m_viewSize);
    
    m_contentScrollView->setDirection(cocos2d::extension::ScrollView::Direction::HORIZONTAL);
    m_contentScrollView->setTouchEnabled(false);
    m_contentScrollView->setContentOffset(Vec2::ZERO);
    m_contentScrollView->setBounceable(true);
    
    m_totalPageCount = static_cast<int>(pContentData->size()) - 1;

    auto container = Layer::create();
    container->setContentSize(Size((m_viewSize.width + 28) * m_totalPageCount, m_viewSize.height));
    m_contentScrollView->setContainer(container);
    
    m_detailScroll_v.clear();
    
    // 设置行间距值
    float line_height;
    float dimens_width;
    switch (AppConfig::getInstance()->_M_LANGUAGE_)
    {
        case Enums::ENGLISH:
            line_height = 45;
            dimens_width = m_viewSize.width - 70;
            break;
            
        case Enums::KOREAN:
            line_height = 55;
            dimens_width = m_viewSize.width - 100;
            break;
    }
    
    
    for (int i=0; i<=m_totalPageCount; i++)
    {
        auto scroll_view = cocos2d::extension::ScrollView::create(m_viewSize);
        
        scroll_view->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
        scroll_view->setTouchEnabled(false);
        scroll_view->setContentOffset(Vec2::ZERO);
        scroll_view->setBounceable(true);
        
        scroll_view->setPosition(Vec2(i*m_viewSize.width + 28, m_viewSize.height));
        scroll_view->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        
        auto content = Label::createWithTTF(pContentData->at(i).data_verse,
                                            "fonts/NanumGothicBold.ttf",
                                            40);
        setPosAndAnchorPointAndTag(content,
                                   Vec2(0, 0),
                                   Vec2::ANCHOR_TOP_LEFT,
                                   0);
        content->setColor(Color3B::WHITE);
        content->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        content->setVerticalAlignment(cocos2d::TextVAlignment::TOP);
        content->setDimensions(dimens_width, 0);
        content->setLineHeight(line_height); // 行间距
        content->setAdditionalKerning(1.5); // 文字间距

//        TLog("label content size: %f, %f", content->getContentSize().width, content->getContentSize().height);
        
        auto sc_container = Layer::create();
        sc_container->setContentSize(Size(m_viewSize.width, content->getContentSize().height));
        scroll_view->setContainer(sc_container);
        
        sc_container->addChild(content);

        container->addChild(scroll_view);
    
        m_detailScroll_v.pushBack(scroll_view);
    }
    
    m_detailDatas_v.clear();
    m_detailDatas_v = *pContentData;
    
    // 创建scroll view的滚动条
    m_labelSVBar = cocos2d::ui::Scale9Sprite::create("bar.jpg");
    setPosAndAnchorPointAndTag(m_labelSVBar,
                               Vec2(m_viewSize.width, m_viewSize.height),
                               Vec2::ANCHOR_TOP_RIGHT,
                               0);
    m_labelSVBar->setCapInsets(Rect(12, 12, 17, 17));
    m_labelSVBar->setContentSize(Size(29, 29));
    
    m_labelSVBar->setScale(0.3f, (m_viewSize.height / 29) / 2);
    m_labelSVBar->setVisible(false);
    
    // 获取滚动条的长度
    m_labelSVBarContentHeight = m_labelSVBar->getScaleY() * 29;
    
    this->addChild(m_labelSVBar);

    this->addChild(m_contentScrollView);
    
    initListener();
}

void DetailContentView::initListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(DetailContentView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(DetailContentView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(DetailContentView::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool DetailContentView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    m_firstPoint    = touch->getLocation();
    m_offset        = m_contentScrollView->getContentOffset();
    
    m_labelScrollView   = m_detailScroll_v.at(m_currentPage);
    m_labelSVOffset     = m_labelScrollView->getContentOffset();
    m_labelHeight       = m_labelScrollView->getChildren().at(0)->getContentSize().height;
    
    if (m_labelHeight > m_viewSize.height)
    {
        // 检测是否可以垂直拖动
        m_verticalEnable = true;
    }
    
    if (!m_contentScrollView->getBoundingBox().containsPoint(this->convertToNodeSpace(m_firstPoint)))
    {
        // If touched content scroll view's outside, then return false.
        return false;
    }
    
    return true;
}

void DetailContentView::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto move_point = touch->getLocation();
    auto distance = move_point - m_firstPoint;
    
    if ((distance.x > 0 && m_currentPage == 0) && (distance.x < 0 && m_currentPage == m_totalPageCount))
    {
        // 当处于第一页向右拖动时或者最后一页向左拖动时，返回不做处理
        return;
    }
    
    if (m_horizontal)
    {
        // 横向拖动模式
        
        if (fabsf(move_point.y - m_firstPoint.y) / fabs(distance.x) > 0.7f)
        {
            // 检测是否是垂直方向移动
            if (m_horizontal && m_verticalEnable)
            {
                m_horizontal = false;
                m_labelSVBar->setVisible(true);
                
                // 显示滚动条
                if (m_labelSVBar->isRunning())
                {
                    m_labelSVBar->stopAllActions();
                    m_labelSVBar->setOpacity(255);
                }else
                {
                    m_labelSVBar->runAction(FadeIn::create(0.2f));
                }
            }
            
        }else
        {
            // 横向移动时
            m_contentScrollView->setContentOffset(Vec2(distance.x + m_offset.x, 0));
            m_verticalEnable = false;
        }
        
    }else
    {
        // 垂直移动模式
        if (m_labelSVBar->isVisible())
        {
            float offset_y = m_labelSVOffset.y + distance.y * 0.4f;
            
            if (fabsf(offset_y) > m_labelHeight)
            {
                offset_y = (offset_y > 0 ? m_labelHeight : -(m_labelHeight));
            }
            
            m_labelScrollView->setContentOffset(Vec2(0, offset_y));
            
            TLog("DetailContentView::onTouchMoved --> detla y: %f", touch->getDelta().y);
            
            if (touch->getDelta().y != 0.0f)
            {
                // 处理滚动条移动
                this->setLabelScrollViewBarContentOffset(touch->getDelta().y);
            }
            
        }
    }
    
}

void DetailContentView::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto end_point = touch->getLocation();
    auto distance = end_point - m_firstPoint;
    
    if (m_horizontal)
    {
        // 横向模式
        if (fabsf(distance.x) < LIMIT_SCROLL_LENGTH)
        {
            if (distance.x < 0)
            {
                m_currentPage--;
            }else if (distance.x > 0)
            {
                m_currentPage++;
            }
        }
        
        this->adjustScrollView(distance.x);
        
    }else
    {
        // 垂直模式
        this->adjustLabelScrollView(m_labelScrollView->getContentOffset().y);
    }
    
    if (m_labelSVBar->isVisible())
    {
        // 当滚动条是可是状态时，进行隐藏操作
        this->hideScrollViewBar();
    }
    
    // 初始化变量
    m_horizontal = true;
    m_verticalEnable = false;
}

/**
 scroll view 滚动条相关函数
 
 当手指离开屏幕后调用
 */
void DetailContentView::hideScrollViewBar()
{
    m_labelSVBar->runAction( Sequence::create(ScaleTo::create(0.2f, 0.3f, (m_viewSize.height / 29) / 2),
                                              DelayTime::create(3.0f),
                                              FadeOut::create(0.2f),
                                              CallFunc::create(LAMBDA_FUNCTION_START
                                                               m_labelSVBar->setVisible(false);
                                                               LAMBDA_FUNCTION_END),
                                              nullptr) );
}

void DetailContentView::adjustLabelScrollView(float distance)
{
    if (distance > m_labelHeight - m_viewSize.height)
    {
        m_labelScrollView->setContentOffsetInDuration(Vec2(0, m_labelHeight - m_viewSize.height), 0.2f);
        
    }else if (distance < 0)
    {
        m_labelScrollView->setContentOffsetInDuration(Vec2(0, 0), 0.2f);
    }
}


void DetailContentView::adjustScrollView(float distance)
{
    TLog("DetailContentView::adjustScrollView --> current page: %d, distance: %f", m_currentPage, distance);
    
    if (distance < 0)
    {
        m_currentPage++;
    }
    else if (distance > 0)
    {
        m_currentPage--;
    }
    
    if (m_currentPage < 0)
    {
        m_currentPage = 0;
    }
    else if (m_currentPage > m_totalPageCount)
    {
        m_currentPage = m_totalPageCount;
    }
    
    auto adjustPoint = Vec2(-m_viewSize.width * m_currentPage, 0);
    m_contentScrollView->setContentOffsetInDuration(adjustPoint, 0.2f);
    
    m_isPageChanged = true;
    
    if (m_labelSVBar->isVisible())
    {
        m_labelSVBar->setVisible(false);
    }
    
    TLog("current page: %d, adjust: %f %f",m_currentPage, adjustPoint.x, adjustPoint.y);
}



DetailDataModel DetailContentView::getCurrentDataModel()
{
    return m_detailDatas_v.at(m_currentPage);
}

/**
 关于scroll view 滚动条的函数
 
 @params deltaY 确定上拉还是下拉的偏移值
 */
void DetailContentView::setLabelScrollViewBarContentOffset(float deltaY)
{
    float y;
    float scale_y = m_labelSVBar->getScaleY() - 0.002f * fabsf((m_viewSize.height - m_labelSVBarContentHeight));
    float scroll_view_offsety = m_labelScrollView->getContentOffset().y;
    
    if (deltaY > 0)
    {
        // 下拉操作
        y = m_labelSVBar->getPositionY() - m_labelHeight / (m_labelHeight - m_viewSize.height) * fabsf(deltaY) * 0.1f;
        m_labelSVBar->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        
        // 普通下拉操作，当到底部时，改变bar的锚点位置
        if (y <= (m_viewSize.height - m_labelSVBarContentHeight))
        {
            y = 0;
            m_labelSVBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            
            if (scale_y >= 1.0f)
            {
                m_labelSVBar->setScaleY(scale_y);
            }
        }
        else
        {
            // 位于顶部并上拉一部分后，下拉时
            // 设置bar‘s scale y的值，到等于原来的长度时为止。
            if (scroll_view_offsety < 0)
            {
                float scale = m_labelSVBarContentHeight / fabsf(scroll_view_offsety);
                
                if (scale > m_labelSVBarContentHeight / 29.0f)
                {
                    scale = m_labelSVBarContentHeight / 29.0f;
                }else if (scale < 1.0f)
                {
                    scale = 1.0f;
                }
                
                m_labelSVBar->setScaleY(scale);
                
                y = m_viewSize.height;
            }
        }
        
    }else if(deltaY < 0)
    {
        // 上拉操作
        // 当位于底部上拉时，重新设置bar的位置，并改变锚点位置
        if (m_labelSVBar->getPositionY() == 0.0f)
        {
            m_labelSVBar->setPositionY(m_viewSize.height - m_labelSVBarContentHeight);
        }
        
        m_labelSVBar->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        
        y = m_labelSVBar->getPositionY() + m_labelHeight / (m_labelHeight - m_viewSize.height) * fabsf(deltaY) * 0.1f;
        
        // 当位于顶部时
        if (y >= m_viewSize.height)
        {
            if (scale_y >= 1.0f)
            {
                m_labelSVBar->setScaleY(scale_y);
            }
            
            y = m_viewSize.height;
            
        }else
        {
            // 位于底部并下拉一部分后，上拉时
            // 设置bar‘s scale y的值，到等于原来的长度时为止。
            if (scroll_view_offsety > m_labelHeight - m_viewSize.height)
            {
                y = 0;
                m_labelSVBar->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
                
                float scale = m_labelSVBarContentHeight / fabsf(scroll_view_offsety - (m_labelHeight-m_viewSize.height));
                
                if (scale > m_labelSVBarContentHeight / 29.0f)
                {
                    scale = m_labelSVBarContentHeight / 29.0f;
                }else if (scale < 1.0f)
                {
                    scale = 1.0f;
                }

                m_labelSVBar->setScaleY(scale);
            }
        }
    }
    
    m_labelSVBar->setPositionY(y);
    
}

















