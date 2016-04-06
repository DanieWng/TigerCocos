//
//  TigerMacros.h
//  Yoohoo
//
//  Created by Wang zhen on 5/12/15.
//
//

#ifndef __Yoohoo__TigerMacros__
#define __Yoohoo__TigerMacros__

#define USING_NS_APP using namespace app
#define NS_APP_BEGIN namespace app{
#define NS_APP_END   }

#define USING_NS_T using namespace Tiger
#define NS_T_BEGIN namespace Tiger{
#define NS_T_END   }

#define CC_TEXTURE_CACHE    Director::getInstance()->getTextureCache()
#define CC_ANIMATION_CACHE  AnimationCache::getInstance()

#define TLog(format, ...) do{CC_BREAK_IF(false); cocos2d::log(format, ##__VA_ARGS__);} while(0)
#define LAMBDA_FUNCTION_START [&](){
#define LAMBDA_FUNCTION_START_PARAM(_PARAM_) [&, _PARAM_](){
#define LAMBDA_FUNCTION_END   }

#define LOAD_SPROTEFRAMECACHE(_TYPE_) SpriteFrameCache::getInstance()->addSpriteFramesWithFile(_TYPE_)
#define LOAD_SPRITEFRAMECACHE_WITH_NAME(_PLIST_, _TEXTURE_) SpriteFrameCache::getInstance()->addSpriteFramesWithFile(_PLIST_, _TEXTURE_)
#define REMOVE_SPRITEFRAMECACHE(_TYPE_) SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(_TYPE_)
#define GET_SPRITEFRAME(_TYPE_) SpriteFrameCache::getInstance()->getSpriteFrameByName(_TYPE_)

#define GET_CHILD_BY_TAG(_TYPE_, _PARENT_, _TAG_) static_cast<_TYPE_>(_PARENT_->getChildByTag(_TAG_))

#define GET_RES_BY_LANGUAGE(format, ...) __String::createWithFormat(format, ##__VA_ARGS__)->getCString()

#define RETURN_DELEGATE_0(_delegate_)               if(_delegate_) _delegate_()
#define RETURN_DELEGATE_1(_delegate_, _p1_)         if(_delegate_) _delegate_(_p1_)
#define RETURN_DELEGATE_2(_delegate_, _p1_, _p2_)   if(_delegate_) _delegate_(_p1_, _p2_)
#define RETURN_DELEGATE_3(_delegate_, _p1_, _p2_, _p3_)   if(_delegate_) _delegate_(_p1_, _p2_, _p3_)

#define SAFE_REMOVE_FROM_PARENT(_POINTER_)  \
    _POINTER_->removeFromParentAndCleanup(true);   \
    _POINTER_ = nullptr;

#define SAFE_REMOVE_LISTENER(_LISTENER_) \
    _eventDispatcher->removeEventListener(_LISTENER_);\
    _LISTENER_ = nullptr;

#define SAFE_FREE_VECTOR(_TYPE_, _VAR_) std::vector<_TYPE_>().swap(_VAR_);

#define TEXTURE2D_USE_RGB565()      Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB565)
#define TEXTURE2D_USE_RGB888()      Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGB888)
#define TEXTURE2D_USE_RGBA8888()    Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888)


#define IF_NULL_THEN_RETUEN(_REF_, _RETURN_) if(_REF_ == nullptr) return _RETURN_


#endif /* defined(__Yoohoo__TigerMacros__) */
