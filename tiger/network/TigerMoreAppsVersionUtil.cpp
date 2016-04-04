//
//  TigerMoreAppsVersionUtil.cpp
//  TigerAPI
//
//  Created by Wang zhen on 4/4/16.
//
//

#include "TigerMoreAppsVersionUtil.hpp"
#include <external/json/rapidjson.h>
#include <external/json/document.h>
#include "BaseAppConfig.hpp"

using namespace Tiger;
using namespace rapidjson;

// inline method begin

static inline const std::string getResponseString(HttpResponse *response)
{
    std::string data = "";
    std::vector<char>* v = response->getResponseData();
    for (int i=0; i<v->size(); i++)
    {
        data.append(cocos2d::__String::createWithFormat("%c", v->at(i))->getCString());
    }
    
    return data;
}

static inline bool checkResponseStatus(HttpResponse *response)
{
    if (!response)
    {
        CCLOG("no response");
        return false;
    }
    
    int status_code = (int)response->getResponseCode();
    char status_string[64] = {};
    sprintf(status_string, "Http Status Code: %d, tag = %s", status_code, response->getHttpRequest()->getTag());
    CCLOG("response code: %s", status_string);
    
    if (status_code == HTTP_ERROR_TIME_OUT ||
        !response->isSucceed())
    {
        CCLOG("\nresponse failed");
        CCLOG("error buffer: %s\n", response->getErrorBuffer());
        return false;
    }
    
    return true;
}

static inline const std::string unzip(const void* data, ssize_t dataSize, const std::string savePath)
{
    const std::string writable_path = savePath;
    
    auto zipfile = cocos2d::ZipFile::createWithBuffer(data, dataSize);
    std::string root_directory = zipfile->getFirstFilename();
    
    for (std::string filename=zipfile->getFirstFilename(); !filename.empty(); filename=zipfile->getNextFilename())
    {
        if (*filename.rbegin() == '/')
        {
            // it's a directory
            cocos2d::FileUtils::getInstance()->createDirectory(writable_path + filename);
        }
        else
        {
            // it's a file
            ssize_t file_size;
            unsigned char* file_data = zipfile->getFileData(filename, &file_size);
            {
                const std::string full_path = writable_path + filename;
                FILE *file = fopen(full_path.c_str(), "wb");
                fwrite(file_data, file_size, 1, file);
                fclose(file);
            }
            free(file_data);
        }
    }
    CC_SAFE_DELETE(zipfile);
    
    return root_directory;
}

// inline method end

TigerMoreAppsVersionUtil* TigerMoreAppsVersionUtil::_instance = nullptr;

TigerMoreAppsVersionUtil* TigerMoreAppsVersionUtil::getInstance()
{
    if (!_instance)
    {
        _instance = new TigerMoreAppsVersionUtil();
    }
    
    return _instance;
}

void TigerMoreAppsVersionUtil::destoryInstance()
{
    if (_instance)
    {
        CC_SAFE_DELETE(_instance);
        _instance = nullptr;
    }
}

TigerMoreAppsVersionUtil::TigerMoreAppsVersionUtil()
{
    _isDownloading = false;
}

TigerMoreAppsVersionUtil::~TigerMoreAppsVersionUtil()
{
}

void TigerMoreAppsVersionUtil::checkVersionBegan()
{
    auto http_client = TigerHttpClient::getInstance();
    
    http_client->setUrl(VERSION_URL);
    http_client->setResponseDelegate(CC_CALLBACK_1(TigerMoreAppsVersionUtil::listenCheckVersionEnded, this));
    http_client->requestGet();
    
    _isDownloading = true;
}

bool TigerMoreAppsVersionUtil::listenCheckVersionEnded(cocos2d::network::HttpResponse *response)
{
    TigerHttpClient::getInstance()->destoryInstance();

    if (!checkResponseStatus(response))
    {
        return false;
    }
    
    std::string data = getResponseString(response);
    
    CCLOG("\nresponse data: %s", data.c_str());
    
    auto new_version_data = parseJson(data.c_str());
    auto cur_version_data = BaseAppConfig::getInstance()->getCurVersionData();
    
    if (new_version_data._version > cur_version_data._version)
    {
        // has new version, should get new
        BaseAppConfig::getInstance()->setCurVersionData(new_version_data);
        BaseAppConfig::getInstance()->setIsDownloadedMoreApps(false);
        BaseAppConfig::getInstance()->saveUserDefault();
        
        downloadMoreAppsBegan();
    }else
    {
        destoryInstance();
    }
    
    return true;
}

VersionData TigerMoreAppsVersionUtil::parseJson(const std::string json)
{
    auto version_data = VersionData();
    
    Document document;
    
    document.Parse<0>(json.c_str());
    
    if (document.HasParseError())
    {
        cocos2d::log("### json document parse error [%u]###", document.GetParseError());
        return version_data;
    }
    
    if (document.IsObject())
    {
        if (document.HasMember(VERSION_JSON_MEMBER_VERSION))
        {
            version_data._version = document[VERSION_JSON_MEMBER_VERSION].GetUint();
        }
        
        if (document.HasMember(VERSION_JSON_MEMBER_LAST_RELEASE_DATE))
        {
            version_data._lastAppReleaseDate = document[VERSION_JSON_MEMBER_LAST_RELEASE_DATE].GetUint();
        }
        
        if (document.HasMember(VERSION_JSON_MEMBER_NEW_DOWNLOAD_URL))
        {
            const std::string base_url = document[VERSION_JSON_MEMBER_NEW_DOWNLOAD_URL].GetString();
            std::string file_name = "";
            if (BaseAppConfig::getInstance()->getSceneScaleFactor() == 2.0f)
            {
                file_name = "2048.zip";
            }
            else if (BaseAppConfig::getInstance()->getSceneScaleFactor() == 1.0f)
            {
                file_name = "1024.zip";
            }
            version_data._resDownloadUrl = base_url + file_name;
        }
    }
    
    return version_data;

}

void TigerMoreAppsVersionUtil::downloadMoreAppsBegan()
{
    auto http_client = TigerHttpClient::getInstance();
    
    http_client->setUrl(BaseAppConfig::getInstance()->getCurVersionData()._resDownloadUrl);
    http_client->setResponseDelegate(CC_CALLBACK_1(TigerMoreAppsVersionUtil::listenDownloadMoreAppsEnded, this));
    
    http_client->requestGet();
}

void TigerMoreAppsVersionUtil::listenDownloadMoreAppsEnded(cocos2d::network::HttpResponse *response)
{
    TigerHttpClient::getInstance()->destoryInstance();
    
    if (!checkResponseStatus(response))
    {
        return;
    }
    
    std::string save_root = FileUtils::getInstance()->getWritablePath()+"moreapps/";
    
    std::string root_directory = unzip(&response->getResponseData()->at(0), response->getResponseData()->size(), save_root);
    
    save_root.append(root_directory);
    
    FileUtils::getInstance()->addSearchPath(save_root);
    
    BaseAppConfig::getInstance()->setIsDownloadedMoreApps(true);
    BaseAppConfig::getInstance()->saveUserDefault();
    
    TLog("listenDownloadMoreAppsEnded -- unzip -> write to save [%s]", save_root.c_str());
    destoryInstance();
}











