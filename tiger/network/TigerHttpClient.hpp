//
//  TigerHttpClient.hpp
//  TigerAPI
//
//  Created by Wang zhen on 1/5/16.
//
//

#ifndef TigerHttpClient_hpp
#define TigerHttpClient_hpp


#include <network/HttpClient.h>
#include <external/json/rapidjson.h>
#include <external/json/document.h>

USING_NS_CC;
using namespace rapidjson;
using namespace cocos2d::network;

#define HTTP_ERROR_TIME_OUT -1


namespace Tiger
{
    static inline const std::string getResponseString(HttpResponse *response)
    {
        std::vector<char>* v = response->getResponseData();
        std::string data(&(v->front()), v->size());
        
        CCLOG("getResponseString : \n%s", data.c_str());
        
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
    
    class TigerHttpClient
    {
    public:
        
        static TigerHttpClient* getInstance();
        void destoryInstance();
        
        TigerHttpClient();
        virtual ~TigerHttpClient();
        
        typedef std::function<void(cocos2d::network::HttpResponse*)> fHttpResponseDelegate;
        
        void setResponseDelegate(fHttpResponseDelegate d);
        
        void requestGet();
        void requestPost();
        void requestPut();
        
        void setUrl(const std::string& url);
        void setHeaders(std::vector<std::string> headers);
        void setPostData(const std::string& data);
        
    protected:
        
    private:
        
        void onHttpResponse(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);
        
    private:
        
        static TigerHttpClient* _instance;
        
        fHttpResponseDelegate _responseDelegate;
        
        std::string _url;
        std::vector<std::string> _headers;
        std::string _postData;
    };
}

#endif /* TigerHttpClient_hpp */




