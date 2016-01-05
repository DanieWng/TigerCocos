//
//  TigerIPLocation.cpp
//  TigerAPI
//
//  Created by Wang zhen on 1/5/16.
//
//

#include "TigerIPLocation.hpp"
#include "../cocos2d/external/json/rapidjson.h"
#include "../cocos2d/external/json/document.h"

using namespace Tiger;
using namespace rapidjson;

TigerIPLocation* TigerIPLocation::_instance = nullptr;

TigerIPLocation::TigerIPLocation()
{
}

TigerIPLocation::~TigerIPLocation()
{
}

TigerIPLocation* TigerIPLocation::getInstance()
{
    if (!_instance)
    {
        _instance = new (std::nothrow)TigerIPLocation();
    }
    
    return _instance;
}

void TigerIPLocation::destoryInstance()
{
    if (_instance)
    {
        CC_SAFE_DELETE(_instance);
        _instance = nullptr;
    }
}

void TigerIPLocation::startGetLocation()
{
    auto http_client = TigerHttpClient::getInstance();
    http_client->setResponseDelegate(CC_CALLBACK_1(TigerIPLocation::doneGetLocation, this));
    http_client->requestGet("http://freegeoip.net/json/");
}

bool TigerIPLocation::doneGetLocation(cocos2d::network::HttpResponse *response)
{
    if (!response)
    {
        cocos2d::log("no response");
        return false;
    }
    
    int status_code = response->getResponseCode();
    char status_string[64] = {};
    sprintf(status_string, "Http Status Code: %d, tag = %s", status_code, response->getHttpRequest()->getTag());
    cocos2d::log("response code: %s", status_string);
    
    if (!response->isSucceed())
    {
        cocos2d::log("response failed");
        cocos2d::log("error buffer: %s", response->getErrorBuffer());
        return false;
    }
    
    std::string data = "";
    std::vector<char>* v = response->getResponseData();
    data = &(v->at(0));
    
    cocos2d::log("response data: %s", data.c_str());
    
    parseJson(data);
    
    TigerHttpClient::destoryInstance();
    
    return true;
}

bool TigerIPLocation::parseJson(const std::string json)
{
    Document document;
    
    document.Parse<0>(json.c_str());
    
    if (document.HasParseError())
    {
        cocos2d::log("### json document parse error ###");
        return false;
    }
    
    IPResponeData data;
    
    if (document.IsObject())
    {
        if (document.HasMember("ip"))
        {
            data._ipAddress = document["ip"].GetString();
        }
        
        if (document.HasMember("country_code"))
        {
            data._countryCode = document["country_code"].GetString();
        }
        
        if (document.HasMember("country_name"))
        {
            data._countryName = document["country_name"].GetString();
        }
        
        if (document.HasMember("region_code"))
        {
            data._regionCode = document["region_code"].GetString();
        }
        
        if (document.HasMember("region_name"))
        {
            data._regionName = document["region_name"].GetString();
        }
        
        if (document.HasMember("city"))
        {
            data._city = document["city"].GetString();
        }
        
        if (document.HasMember("zip_code"))
        {
            data._zipCode = document["zip_code"].GetString();
        }
        
        if (document.HasMember("time_zone"))
        {
            data._timeZone = document["time_zone"].GetString();
        }
        
        if (document.HasMember("latitude"))
        {
            data._latitude = document["latitude"].GetDouble();
        }
        
        if (document.HasMember("longitude"))
        {
            data._longitude = document["longitude"].GetDouble();
        }
        
        if (document.HasMember("metro_code"))
        {
            data._metroCode = document["metro_code"].GetInt();
        }
    }
    
    return true;
}






