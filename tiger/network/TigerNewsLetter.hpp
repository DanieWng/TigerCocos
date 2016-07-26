//
//  TigerNewsLetter.hpp
//  TigerAPI
//
//  Created by Wang zhen on 4/4/16.
//
//

#ifndef TigerNewsLetter_hpp
#define TigerNewsLetter_hpp

#include "TigerHttpClient.hpp"
#include "../../AppData.h"

namespace Tiger
{
    struct NewsLetterMemberData
    {
        std::string _mailMD5;
        std::string _mail;
        std::string _firstName;
        std::string _lastName;
    };
    
    class TigerNewsLetterHttpDelegate
    {
    public:
        
        virtual void putNewsLetterHttpStatusCode(const int code) {};
    };
    
    class TigerNewsLetter
    {
    public:
        
        TigerNewsLetter();
        ~TigerNewsLetter();
        
        static TigerNewsLetter* getInstance();
        void destoryInstance();
        
        void addSubscriberToListID(const std::string& dataCenter,
                                   const std::string& listID,
                                   const NewsLetterMemberData& data,
                                   const bool isRunBackground);
        /*
        Listen http callback
         */
        bool listenHttpResult(HttpResponse *response);
        
        void setDelegate(TigerNewsLetterHttpDelegate* d);
        
    private:
        
        void doPutEvent(const std::string& url, const NewsLetterMemberData& data);
        
    private:
        
        static TigerNewsLetter* _instance;
    
        TigerNewsLetterHttpDelegate *_delegate;
    };
    
};


#endif /* TigerNewsLetter_hpp */











