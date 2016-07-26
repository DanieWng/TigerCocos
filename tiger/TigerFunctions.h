//
//  TigerFunctions.h
//  Yoohoo
//
//  Created by Wang zhen on 5/13/15.
//
//

#ifndef Yoohoo_TigerFunctions_h
#define Yoohoo_TigerFunctions_h

#include "cocos2d.h"
#include "TCustomToast.h"
#include <ui/CocosGUI.h>
#include "TigerEnum.h"
#include "TigerMacros.h"
#include "TigerNativeFunction.h"

#include <iostream>
#include <cstdlib>
#include "md5/md5.h"

#include <external/json/rapidjson.h>
#include <external/json/document.h>

using namespace rapidjson;

using namespace cocos2d::ui;
USING_NS_CC;

namespace Tiger
{
    static inline bool parseJsonData(Document& document, const std::string& data)
    {
        document.Parse<0>(data.c_str());
        if (document.HasParseError())
        {
            cocos2d::log("### json document parse error [%u]###", document.GetParseError());
            return false;
        }
        
        return true;
    }
    
    static inline std::string getMD5(const std::string str)
    {
        md5_state_t state;
        md5_byte_t digest[16];
        char hex_output[16*2 + 1];
        int di;
        
        md5_init(&state);
        md5_append(&state, (const md5_byte_t *)str.c_str(), (int)str.length());
        md5_finish(&state, digest);
        
        for (di=0; di<16; ++di)
        {
            sprintf(hex_output + di*2, "%02x", digest[di]);
        }
        
        return hex_output;
    }
    
    inline std::string getCurrentDate()
    {
        struct tm* tm;
        time_t timep;
        
        timeval tv;
        gettimeofday(&tv, nullptr);
        timep = tv.tv_sec;
        
        tm = localtime(&timep);
        int year = tm->tm_year + 1900;
        int month = tm->tm_mon + 1;
        int day = tm->tm_mday;
        //    int h = tm->tm_hour;
        //    int m = tm->tm_min;
        //    int s = tm->tm_sec;
        
        //        TLog("%d/%d/%d/day", year, month, day);
        
        return __String::createWithFormat("%d/%d/%d", year, month, day)->getCString();
    }
    
    inline double getCurrentTime()
    {
        time_t timep;
        
        timeval tv;
        gettimeofday(&tv, nullptr);
        timep = tv.tv_sec;
        
        return timep;
    }
    
    /**
     getTime - return sec
     */
    inline double getTime()
    {
        timeval tv;
        gettimeofday(&tv, nullptr);
        
        return tv.tv_sec + tv.tv_usec*pow(10, -6);
    }
    
    inline std::string getCurYear()
    {
        struct tm* tm;
        time_t timep;
        
        timeval tv;
        gettimeofday(&tv, nullptr);
        timep = tv.tv_sec;
        
        tm = localtime(&timep);
        int year = tm->tm_year + 1900;
        
        return __String::createWithFormat("%d", year)->getCString();
    }
    
    inline std::string translateToDate(double sec)
    {
        struct tm* tm;
        
        time_t timep = sec;
        tm = localtime(&timep);
        int year = tm->tm_year + 1900;
        int month = tm->tm_mon + 1;
        int day = tm->tm_mday;
        
        return __String::createWithFormat("%d / %d / %d", year, month, day)->getCString();
    }
    
    inline void checkWritablePathIsInside()
    {
        bool is_exist_write_path = false;
        
        auto search_paths = FileUtils::getInstance()->getSearchPaths();
        
        for (auto path : search_paths)
        {
            TLog("search path [%s]", path.c_str());
            
            if (path == FileUtils::getInstance()->getWritablePath())
            {
                is_exist_write_path = true;
            }
        }
        
        if (!is_exist_write_path)
        {
            FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath());
            
            TLog("added search path");
        }
    }
    
    inline std::string computeDaysAboutLastFlight(double lastFlightDate,
                                                  const cocos2d::LanguageType language = cocos2d::LanguageType::ENGLISH)
    {
        time_t cur_time;
        timeval tv;
        gettimeofday(&tv, nullptr);
        cur_time = tv.tv_sec;
        
        double diff = cur_time - lastFlightDate;
        
        // day
        double min   = diff / 60;
        double hour  = min / 60;
        double day   = hour / 24;
        //        double month = day / 30;
        double year  = day / 365;
        
        std::string time_str;
        
        std::string text_min = "minute(s)";
        std::string text_hour = "hour(s)";
        std::string text_day = "day(s)";
        std::string text_year = "year(s)";
        
        switch (language)
        {
            case cocos2d::LanguageType::KOREAN:
                text_min = "분";
                text_hour = "시간";
                text_day = "일";
                text_year = "년";
                break;
                
            case cocos2d::LanguageType::CHINESE:
                text_min = "分钟";
                text_hour = "小时";
                text_day = "天";
                text_year = "年";
                break;
                
            case cocos2d::LanguageType::JAPANESE:
                text_min = "分钟";
                text_hour = "時間";
                text_day = "日";
                text_year = "年";
                break;
                
            case cocos2d::LanguageType::RUSSIAN:
                text_min = "минут";
                text_hour = "час(ов)";
                text_day = "дней";
                text_year = "лет";
                break;
                
            case cocos2d::LanguageType::ITALIAN:
                text_min = "minuto/i";
                text_hour = "ora/e";
                text_day = "giorno/i";
                text_year = "anno/I";
                break;
                
            case cocos2d::LanguageType::GERMAN:
                text_min = "Minute(n)";
                text_hour = "Stunde(n)";
                text_day = "Tag(en)";
                text_year = "Jahr(en)";
                break;
                
            case cocos2d::LanguageType::FRENCH:
                text_min = "minute(s)";
                text_hour = "heure(s)";
                text_day = "jour(s)";
                text_year = "an(s)";
                break;
                
            case cocos2d::LanguageType::SPANISH:
                text_min = "minuto(s)";
                text_hour = "hora(s)";
                text_day = "día(s)";
                text_year = "año(s)";
                break;
                
            default:
                break;

        }
        
        if (min < 60)
        {
            time_str = __String::createWithFormat("%d %s", (int)min, text_min.c_str())->getCString();
        }else if (hour >= 1 && hour < 24)
        {
            time_str = __String::createWithFormat("%d %s", (int)hour, text_hour.c_str())->getCString();
        }else if (day >= 1 && day <= 365)
        {
            time_str = __String::createWithFormat("%d %s", (int)day, text_day.c_str())->getCString();
        }else if (year >= 1)
        {
            time_str = __String::createWithFormat("%d %s", (int)year, text_year.c_str())->getCString();
        }
        
        return time_str;
    }
    
    inline int computeDaysWithLastDate(double lastDate)
    {
        time_t cur_time;
        timeval tv;
        gettimeofday(&tv, nullptr);
        cur_time = tv.tv_sec;
        
        double diff = cur_time - lastDate;
        
        // day
        double min   = diff / 60;
        double hour  = min / 60;
        double day   = hour / 24;
        
        return day;
    }
    
    inline void setPosAndAnchorPointAndTag(Ref* target, Vec2 pos, Vec2 anchor, int tag)
    {
        auto t = (Node*)target;
        t->setPosition(pos);
        t->setAnchorPoint( anchor );
        t->setTag(tag);
    }
    
    inline void setPosBaseRetina(Ref* target, const Vec2 pos, const Vec2 anchor, float sceneScale, bool isToLayer=true)
    {
        Size visible_size = Director::getInstance()->getVisibleSize();
        
        float dif_x = 0.0f;
        
        if (isToLayer)
        {
            if (visible_size.width > (2048 * sceneScale))
            {
                dif_x = (visible_size.width - (2048 * sceneScale)) / 2.0f;
            }
        }
        
        auto t = (Node*)target;
        t->setPosition(Vec2(pos.x * sceneScale + dif_x, pos.y * sceneScale));
        t->setAnchorPoint(anchor);
    }
    
    /**
     dynamicConvertWorldPosBaseRetina
     转换世界坐标，基于retina， 锚点是vec2（0， 0）
     */
    inline Vec2 dynamicConvertWorldPosBaseRetina(const Vec2 p, float sceneScale, bool isComputeDif=true)
    {
        Size visible_size = Director::getInstance()->getVisibleSize();
        
        float dif_x = 0.0f;
        
        if (isComputeDif)
        {
            if (visible_size.width > (2048 * sceneScale))
            {
                dif_x = (visible_size.width - (2048 * sceneScale)) / 2.0f;
            }
        }
        
        return Vec2(p.x * sceneScale + dif_x, p.y * sceneScale);
    }
    
    /** 设置Button的enable值
     */
    inline void setButtonIsEnable(Button *btn, bool isEnable)
    {
        if (isEnable)
        {
            btn->setBright(true);
            btn->setEnabled(true);
            btn->setTouchEnabled(true);
        }else
        {
            btn->setBright(false);
            btn->setEnabled(false);
            btn->setTouchEnabled(false);
        }
    }
    
    inline Button* createButton(const std::string& normal,
                                const std::string& select,
                                const std::string& disable,
                                const cocos2d::ui::AbstractCheckButton::ccWidgetTouchCallback &callback=nullptr,
                                Widget::TextureResType resType=Widget::TextureResType::LOCAL)
    {
        auto btn = Button::create(normal, select, disable, resType);
        if (callback)
        {
            btn->addTouchEventListener(callback);
        }
        
        return btn;
    }
    
    /**
     In android, the data file must in writeable path. If not, can open the data file.
     @parma filePath ex:"asset/xxx.sqlite"
     @parma fileName ex:"xxx.sqlite"
     */
    inline void copyFileToSave(const std::string& assetPath, const std::string& saveTo)
    {
        // Check is file exist in resources folder.
        CCASSERT(FileUtils::getInstance()->isFileExist(assetPath), "");
        
        if (FileUtils::getInstance()->isFileExist(saveTo))
            FileUtils::getInstance()->removeFile(saveTo);
        
        // Create file in writeable path.
        FILE* out_file = fopen(saveTo.c_str(), "r");
        if (out_file == nullptr)
        {
            // Get data from DB file.
            ssize_t size;
            const char* data = (char*)cocos2d::FileUtils::getInstance()->getFileData(assetPath.c_str(),
                                                                                     "rb",
                                                                                     &size);
            
            // Ready wirte data into the out file.
            out_file = fopen(saveTo.c_str(), "wb");
            
            // Write data into file.
            fwrite(data, size, 1, out_file);
            
            CC_SAFE_DELETE_ARRAY(data);
        }
        
        fclose(out_file);
        
        TLog("\n-- copy to [%s] --", saveTo.c_str());
    }
    
}



#endif
