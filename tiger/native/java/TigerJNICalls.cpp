//
//  TigerJNICalls.cpp
//  TheSolarSystem
//
//  Created by Wang zhen on 10/28/14.
//  Copyright (c) 2014 Y Factory. All rights reserved.
//

#include "TigerJNICalls.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"

#define NDK_HELPER_JAVA                     "org/cocos2dx/cpp/TigerCocos2dActivity"
#define CALL_MAIL_METHOD                    "callMail"
#define CALL_MAIL_WITH_IMAGE_METHOD         "trySendAnMailWithImage"
#define CALL_WEB_METHOD                     "callWeb"
#define CALL_IS_CONNECTION                  "callIsInternetConnection"
#define CALL_GET_VAILABLE_MEMORY_SIZE       "tryGetAvailableInternalMemorySize"
#define CALL_SAVE_IMAGE_TO_SD_METHOD        "trySaveImageToSdcard"
#define CALL_SAVE_IMAGE_TO_ALBUM_METHOD     "trySaveImageToPhotoAlbum"
#define CALL_OPEN_URL                       "tryOpenURL"
#define CALL_SEND_SCENE_NAME_TO_GAI         "trySendSceneNameToGoogleAnalytics"


void TigerJNICalls::trySendAnEmail(const char *addres, const char *title, const char *body)
{
    
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_MAIL_METHOD,
                                                           "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    
    if (isExist)
    {

        jstring a = jmi.env->NewStringUTF(addres);
        jstring t = jmi.env->NewStringUTF(title);
        jstring b = jmi.env->NewStringUTF(body);

        jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, a, t, b);
        

    }else
    {
        return;
    }
    
}

void TigerJNICalls::trySendAnEmailWithImage(const char* address, const char* title, const char* body, const char* imagefile)
{
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_MAIL_WITH_IMAGE_METHOD,
                                                           "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (isExist)
    {
        jstring a = jmi.env->NewStringUTF(address);
        jstring t = jmi.env->NewStringUTF(title);
        jstring b = jmi.env->NewStringUTF(body);
        jstring i = jmi.env->NewStringUTF(imagefile);
        
        jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, a, t, b, i);
    }

}


void TigerJNICalls::tryCallWeb(const char* urlStr)
{
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_WEB_METHOD,
                                                           "(Ljava/lang/String;)V");
    
    if (!isExist)
    {
        return;
    }else
    {
        jstring urlJS = jmi.env->NewStringUTF(urlStr);
        
        jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, urlJS);

    }
}


bool TigerJNICalls::tryIsInternetcConnection()
{
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_IS_CONNECTION,
                                                           "()Z");
    if (isExist)
    {
        return jmi.env->CallStaticBooleanMethod(jmi.classID, jmi.methodID);
    }
    
}


long TigerJNICalls::tryGetVailableMemorySize()
{
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_GET_VAILABLE_MEMORY_SIZE,
                                                           "()J");
    
    if (isExist)
    {
        long l = (jlong)jmi.env->CallStaticLongMethod(jmi.classID, jmi.methodID);
        
        return l;
    }
}

void TigerJNICalls::trySaveImageToPhotoAlbum(const char* fileName)
{
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_SAVE_IMAGE_TO_ALBUM_METHOD,
                                                           "(Ljava/lang/String;)V");
    
    if (isExist)
    {
        jstring js = jmi.env->NewStringUTF(fileName);
        jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, js);
    }
}

const char* TigerJNICalls::trySaveImageToSdcard(const char* fileName)
{
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_SAVE_IMAGE_TO_SD_METHOD,
                                                           "(Ljava/lang/String;)Ljava/lang/String;");
    
    if (isExist)
    {
        jstring js = jmi.env->NewStringUTF(fileName);
        jstring sd_path = (jstring)jmi.env->CallStaticObjectMethod(jmi.classID, jmi.methodID, js);
        
        const char* path_str = jmi.env->GetStringUTFChars(sd_path, JNI_FALSE);
        return path_str;
    }
}

void TigerJNICalls::tryOpenUrl(const char* url)
{
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_OPEN_URL,
                                                           "(Ljava/lang/String;)V");
    
    if (isExist)
    {
        jstring js = jmi.env->NewStringUTF(url);
        jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, js);
    }
}

void TigerJNICalls::trySendSceneNameToGoogleAnalytics(const char* scene)
{
    cocos2d::JniMethodInfo jmi;
    
    bool isExist = cocos2d::JniHelper::getStaticMethodInfo(jmi,
                                                           NDK_HELPER_JAVA,
                                                           CALL_SEND_SCENE_NAME_TO_GAI,
                                                           "(Ljava/lang/String;)V");
    
    if (isExist)
    {
        jstring js = jmi.env->NewStringUTF(scene);
        jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, js);
    }
}














