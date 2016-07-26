//
//  TigerMoreAppsVersionUtil.hpp
//  TigerAPI
//
//  Created by Wang zhen on 4/4/16.
//
//

#ifndef TigerMoreAppsVersionUtil_hpp
#define TigerMoreAppsVersionUtil_hpp

#include "TigerHttpClient.hpp"
#include "../../AppData.h"

#define MORE_APPS_ROOT_PATH FileUtils::getInstance()->getWritablePath()+"moreapps/"

namespace Tiger
{
    struct VersionData
    {
        unsigned int _version;
        unsigned int _lastAppReleaseDate;
        std::string _resDownloadUrl;
        
        VersionData()
        {
            _version = 0;
            _lastAppReleaseDate = 0;
            _resDownloadUrl = "";
        }
    };
    
    enum class MoreAppsVersionStatus
    {
        kIsLastVersion,
        kIsHaveNewVersion,
        kIsDownloadBegan,
        kIsUnzipBegan,
        kIsDownloadNewResEnded,
        kIsDownloadError,
    };
    
    class TigerMoreAppsVersionUtil
    {
    public:
        
        TigerMoreAppsVersionUtil();
        ~TigerMoreAppsVersionUtil();
        
        static TigerMoreAppsVersionUtil* getInstance();
        static void destoryInstance();
        
        void checkVersionBegan(bool isChina=false);
        
        void downloadMoreAppsBegan();
        
        typedef std::function<void(MoreAppsVersionStatus)> fMoreAppsVersionStatusDelegate;
        void setDelegate(const fMoreAppsVersionStatusDelegate d);
        
    private:
        
        bool listenCheckVersionEnded(HttpResponse *response);
        VersionData parseJson(const std::string json);
        
        void listenDownloadMoreAppsEnded(HttpResponse *response);
        
    private:
        
        static TigerMoreAppsVersionUtil* _instance;
        
        CC_SYNTHESIZE_READONLY(bool, _isDownloading, IsDownloading);
        
        fMoreAppsVersionStatusDelegate _statusdelegate;
    };
};

#endif /* TigerMoreAppsVersionUtil_hpp */






