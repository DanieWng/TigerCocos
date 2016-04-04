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
    
    class TigerMoreAppsVersionUtil
    {
    public:
        
        TigerMoreAppsVersionUtil();
        ~TigerMoreAppsVersionUtil();
        
        static TigerMoreAppsVersionUtil* getInstance();
        void destoryInstance();
        
        void checkVersionBegan();
        
        void downloadMoreAppsBegan();
        
    private:
        
        bool listenCheckVersionEnded(HttpResponse *response);
        VersionData parseJson(const std::string json);
        
        void listenDownloadMoreAppsEnded(HttpResponse *response);
        
    private:
        
        static TigerMoreAppsVersionUtil* _instance;
        
        CC_SYNTHESIZE_READONLY(bool, _isDownloading, IsDownloading);
    };
};

#endif /* TigerMoreAppsVersionUtil_hpp */






