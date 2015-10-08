//
//  TigerMacros.h
//  Yoohoo
//
//  Created by Wang zhen on 5/12/15.
//
//

#ifndef __Yoohoo__TigerMacros__
#define __Yoohoo__TigerMacros__

#define KEY_USERDEFAULT_IS_EXIST    "userdefault_is_exist"
#define KEY_CUR_LANGUAGE            "cur_language"
#define KEY_CUR_SOUND_ENABLE        "sound_enable"

#define USING_NS_APP using namespace app
#define NS_APP_BEGIN namespace app{
#define NS_APP_END   }

#define USING_NS_T using namespace T
#define NS_T_BEGIN namespace T{
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

#define SAFE_REMOVE_FROM_PARENT(_POINTER_) if(_POINTER_)_POINTER_->removeFromParentAndCleanup(true);

#endif /* defined(__Yoohoo__TigerMacros__) */
