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

using namespace cocos2d::network;

#define HTTP_ERROR_TIME_OUT -1


namespace Tiger
{
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




