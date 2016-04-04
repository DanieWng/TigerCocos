//
//  TigerNewsLetter.cpp
//  TigerAPI
//
//  Created by Wang zhen on 4/4/16.
//
//

#include "TigerNewsLetter.hpp"
#include <external/json/rapidjson.h>
#include <external/json/document.h>
#include <external/json/stringbuffer.h>
#include <external/json/writer.h>
#include "cocos2d.h"

using namespace rapidjson;
using namespace Tiger;
USING_NS_CC;

TigerNewsLetter* TigerNewsLetter::_instance = nullptr;

TigerNewsLetter* TigerNewsLetter::getInstance()
{
    if (!_instance)
    {
        _instance = new TigerNewsLetter();
    }
    
    return _instance;
}

void TigerNewsLetter::destoryInstance()
{
    if (_instance)
    {
        CC_SAFE_DELETE(_instance);
        _instance = nullptr;
    }
}

TigerNewsLetter::TigerNewsLetter()
{
    _isRunBackground = false;
}

TigerNewsLetter::~TigerNewsLetter()
{
}

void TigerNewsLetter::addSubscriberToListID(const std::string &dataCenter,
                                            const std::string &listID,
                                            const Tiger::NewsLetterMemberData &data,
                                            const bool isRunBackground)
{
    _isRunBackground = isRunBackground;
    
    auto http_client = TigerHttpClient::getInstance();
    
    auto url = __String::createWithFormat("https://%s.api.mailchimp.com/3.0/lists/%s/members/%s",
                                          dataCenter.c_str(),
                                          listID.c_str(),
                                          data._mailMD5.c_str())->getCString();
    http_client->setUrl(url);
    
    std::vector<std::string> header;
    header.clear();
    header.push_back(__String::createWithFormat("Authorization: apikey %s", MAILCHIMP_API_KEY)->getCString());
    http_client->setHeaders(header);

    auto create_json = [&](NewsLetterMemberData d)->const std::string{
        Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        
        // write json, 字符串需要转换为常量
        document.AddMember("email_address", rapidjson::Value(d._mail.c_str(), allocator), allocator);
        document.AddMember("status", "subscribed", allocator);
        
        rapidjson::Value merge_field_object(rapidjson::kObjectType);
        merge_field_object.AddMember("FNAME", rapidjson::Value(d._firstName.c_str(), allocator), allocator);
        merge_field_object.AddMember("LNAME", rapidjson::Value(d._lastName.c_str(), allocator), allocator);
        
        document.AddMember("merge_fields", merge_field_object, allocator);
        
        StringBuffer buffer;
        rapidjson::Writer<StringBuffer> writer(buffer);
        document.Accept(writer);
        
        return buffer.GetString();
    };

    http_client->setPostData(create_json(data));
    
    http_client->setResponseDelegate(CC_CALLBACK_1(TigerNewsLetter::listenHttpResult, this));
    
    http_client->requestPut();
}

bool TigerNewsLetter::listenHttpResult(cocos2d::network::HttpResponse *response)
{
    TigerHttpClient::getInstance()->destoryInstance();
    
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
    else if (status_code == MAILCHIMP_API_ERROR_401)
    {
        CCLOG("\nAPI Key Missing");
    }
    else if (status_code == MAILCHIMP_API_ERROR_400)
    {
        CCLOG("\nMember Exists");
    }
    
    std::string data = "";
    std::vector<char>* v = response->getResponseData();
    for (int i=0; i<v->size(); i++)
    {
        data.append(cocos2d::__String::createWithFormat("%c", v->at(i))->getCString());
    }
    
    CCLOG("\nresponse data: %s", data.c_str());
    
    return true;
}
























