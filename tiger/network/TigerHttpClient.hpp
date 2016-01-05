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

namespace Tiger
{
    class TigerHttpClient
    {
    public:
        
        static TigerHttpClient* getInstance();
        static void destoryInstance();
        
        TigerHttpClient();
        virtual ~TigerHttpClient();
        
        typedef std::function<void(cocos2d::network::HttpResponse*)> fHttpResponseDelegate;
        
        void setResponseDelegate(fHttpResponseDelegate d);
        
        void requestGet(const std::string url);
        
    protected:
        
    private:
        
        void onHttpResponse(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);
        
    private:
        
        static TigerHttpClient* _instance;
        
        fHttpResponseDelegate _responseDelegate;
    };
}

#endif /* TigerHttpClient_hpp */
