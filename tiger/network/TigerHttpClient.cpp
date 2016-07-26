//
//  TigerHttpClient.cpp
//  TigerAPI
//
//  Created by Wang zhen on 1/5/16.
//
//

#include "TigerHttpClient.hpp"

using namespace Tiger;


TigerHttpClient* TigerHttpClient::_instance = nullptr;

TigerHttpClient::TigerHttpClient()
{
    _responseDelegate = nullptr;
    
    _url = "";
    _headers.clear();
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

void TigerHttpClient::requestGet()
{
    HttpRequest *request = new HttpRequest();
    
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(_url.c_str());
    
    if (_headers.size() > 0)
    {
        request->setHeaders(_headers);
    }
    
    request->setResponseCallback(CC_CALLBACK_2(TigerHttpClient::onHttpResponse, this));
    
    HttpClient::getInstance()->setTimeoutForConnect(3600);
    HttpClient::getInstance()->setTimeoutForRead(3600);
    HttpClient::getInstance()->send(request);
    
    request->release();
}

void TigerHttpClient::requestPost()
{
    HttpRequest *request = new HttpRequest();
    
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setUrl(_url.c_str());
    
    if (_headers.size() > 0)
    {
        request->setHeaders(_headers);
    }
    
    request->setRequestData(_postData.c_str(), strlen(_postData.c_str()));
    
    request->setResponseCallback(CC_CALLBACK_2(TigerHttpClient::onHttpResponse, this));
    
    HttpClient::getInstance()->send(request);
    
    request->release();
}

void TigerHttpClient::requestPut()
{
    HttpRequest *request = new HttpRequest();
    
    request->setRequestType(cocos2d::network::HttpRequest::Type::PUT);
    request->setUrl(_url.c_str());
    
    if (_headers.size() > 0)
    {
        request->setHeaders(_headers);
    }
    
    request->setRequestData(_postData.c_str(), strlen(_postData.c_str()));
    
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

void TigerHttpClient::setUrl(const std::string &url)
{
    _url = url;
}

void TigerHttpClient::setHeaders(std::vector<std::string> headers)
{
    _headers = headers;
}

void TigerHttpClient::setPostData(const std::string &data)
{
    _postData = data;
}








