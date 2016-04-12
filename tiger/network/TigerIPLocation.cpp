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
    _resultDelegate = nullptr;
}

TigerIPLocation::~TigerIPLocation()
{
    _resultDelegate = nullptr;
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
    http_client->setUrl("http://ip-api.com/json"); // the url is derparture
    http_client->requestGet();
}

bool TigerIPLocation::doneGetLocation(cocos2d::network::HttpResponse *response)
{
    IPResponeData ip_data = IPResponeData();
    
    if (!checkResponseStatus(response))
    {
        _resultDelegate(ip_data);
        return false;
    }
    
    std::string data = getResponseString(response);
    
    ip_data = parseJson(data);
    
    if (_resultDelegate)
    {
        _resultDelegate(ip_data);
    }
    
    TigerHttpClient::getInstance()->destoryInstance();
    
    return true;
}

IPResponeData TigerIPLocation::parseJson(const std::string json)
{
    IPResponeData data = IPResponeData();
    
    Document document;
    
    document.Parse<0>(json.c_str());
    
    if (document.HasParseError())
    {
        cocos2d::log("### json document parse error [%u]###", document.GetParseError());
        return data;
    }
    
    if (document.IsObject())
    {
        if (document.HasMember("countryCode"))
        {
            data._countryCode = document["countryCode"].GetString();
        }
    }
    
    return data;
}

void TigerIPLocation::setResultDelegate(const fIPLocationResultDelegate d)
{
    _resultDelegate = d;
}




