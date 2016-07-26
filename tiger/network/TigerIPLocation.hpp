//
//  TigerIPLocation.hpp
//  TigerAPI
//
//  Created by Wang zhen on 1/5/16.
//
//  r - 0.0.1

#ifndef TigerIPLocation_hpp
#define TigerIPLocation_hpp

#include "TigerHttpClient.hpp"

namespace Tiger {
    
struct IPResponeData
{
//    std::string _as;
//    std::string _city;
//    std::string _country;
    std::string _countryCode;
//    std::string _isp;
//    float       _latitude;
//    float       _longitude;
//    std::string _org;
//    std::string _query;
//    std::string _region;
//    std::string _regionName;
//    std::string _status;
//    std::string _timeZone;
//    std::string _zip;
    
    IPResponeData()
    {
        _countryCode = "";
    }
};
    
class TigerIPLocation
{
public:
    
    TigerIPLocation();
    virtual ~TigerIPLocation();
    
    static TigerIPLocation* getInstance();
    static void destoryInstance();
    
    void startGetLocation();
    bool doneGetLocation(cocos2d::network::HttpResponse *response);
    
    IPResponeData parseJson(const std::string json);
    
    typedef std::function<void(IPResponeData)> fIPLocationResultDelegate;
    
    void setResultDelegate(const fIPLocationResultDelegate d);
    
private:
    
    static TigerIPLocation* _instance;

    fIPLocationResultDelegate _resultDelegate;
};

}

#endif /* TigerIPLocation_hpp */




















