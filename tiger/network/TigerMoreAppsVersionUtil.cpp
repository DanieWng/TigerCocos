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
#include "../TigerFunctions.h"

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
    _statusdelegate = nullptr;
}

TigerMoreAppsVersionUtil::~TigerMoreAppsVersionUtil()
{
    _statusdelegate = nullptr;
}

void TigerMoreAppsVersionUtil::checkVersionBegan(bool isChina)
{
    auto http_client = TigerHttpClient::getInstance();
    
    http_client->setUrl(isChina?VERSION_URL_ALIYUN:VERSION_URL_AWS);
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
    
    auto new_version_data = parseJson(data.c_str());
    
    if (new_version_data._version == 0)
    {
        TLog("get version data faild");
        return false;
    }
    
    BaseAppConfig::getInstance()->setLastConnectServerData(Tiger::getCurrentTime());
    BaseAppConfig::getInstance()->saveUserDefault();
    
    auto cur_version_data = BaseAppConfig::getInstance()->getCurVersionData();
    
    /*
     1.if has a new version
     2.check 'more apps' res floder is exist,
     */
    if (new_version_data._version > cur_version_data._version ||
        !FileUtils::getInstance()->isDirectoryExist(MORE_APPS_ROOT_PATH))
    {
        RETURN_DELEGATE_1(_statusdelegate, MoreAppsVersionStatus::kIsHaveNewVersion);
        
        // has new version, should get new
        BaseAppConfig::getInstance()->setCurVersionData(new_version_data);
        BaseAppConfig::getInstance()->saveUserDefault();
        UserDefault::getInstance()->flush();
        
        downloadMoreAppsBegan();
    }
    else
    {
        RETURN_DELEGATE_1(_statusdelegate, MoreAppsVersionStatus::kIsLastVersion);
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
    
    RETURN_DELEGATE_1(_statusdelegate, MoreAppsVersionStatus::kIsDownloadBegan);
}

void TigerMoreAppsVersionUtil::listenDownloadMoreAppsEnded(cocos2d::network::HttpResponse *response)
{
    TigerHttpClient::getInstance()->destoryInstance();
    
    if (!checkResponseStatus(response) ||
        response->getResponseData()->size() == 0)
    {
        RETURN_DELEGATE_1(_statusdelegate, MoreAppsVersionStatus::kIsDownloadError);
        return;
    }
    
    RETURN_DELEGATE_1(_statusdelegate, MoreAppsVersionStatus::kIsUnzipBegan);
    
    std::string save_root = MORE_APPS_ROOT_PATH;
    
    std::string root_directory = unzip(&response->getResponseData()->at(0), response->getResponseData()->size(), save_root);
    
    TLog("unzip end -- %s", root_directory.c_str());
    
    save_root.append(root_directory);
    
    FileUtils::getInstance()->addSearchPath(save_root);
    
    TLog("listenDownloadMoreAppsEnded -- unzip -> write to save [%s]", save_root.c_str());
    
    RETURN_DELEGATE_1(_statusdelegate, MoreAppsVersionStatus::kIsDownloadNewResEnded);
    
    destoryInstance();
}

void TigerMoreAppsVersionUtil::setDelegate(const fMoreAppsVersionStatusDelegate d)
{
    _statusdelegate = d;
}










