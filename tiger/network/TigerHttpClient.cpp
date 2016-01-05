//
//  TigerHttpClient.cpp
//  TigerAPI
//
//  Created by Wang zhen on 1/5/16.
//
//

#include "TigerHttpClient.hpp"

using namespace Tiger;
using namespace cocos2d::network;

TigerHttpClient* TigerHttpClient::_instance = nullptr;

TigerHttpClient::TigerHttpClient()
{
    _responseDelegate = nullptr;
}

TigerHttpClient::~TigerHttpClient()
{
    _responseDelegate = nullptr;
}

TigerHttpClient* TigerHttpClient::getInstance()
{
    if (!_instance)
    {
        _instance = new (std::nothrow)TigerHttpClient();
    }
    
    return _instance;
}

void TigerHttpClient::destoryInstance()
{
    if (_instance)
    {
        CC_SAFE_DELETE(_instance);
        _instance = nullptr;
    }
}

void TigerHttpClient::setResponseDelegate(fHttpResponseDelegate d)
{
    _responseDelegate = d;
}

void TigerHttpClient::requestGet(const std::string url)
{
    HttpRequest *request = new HttpRequest();
    
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    
    request->setResponseCallback(CC_CALLBACK_2(TigerHttpClient::onHttpResponse, this));
    
    HttpClient::getInstance()->send(request);
    
    request->release();
}

void TigerHttpClient::onHttpResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (_responseDelegate)
    {
        _responseDelegate(response);
    }
}








