//
//  TigerIPLocation.hpp
//  TigerAPI
//
//  Created by Wang zhen on 1/5/16.
//
//

#ifndef TigerIPLocation_hpp
#define TigerIPLocation_hpp

#include "TigerHttpClient.hpp"

namespace Tiger {

enum class CountryType
{
    cChina,
    cKorea,
    cOther,
    cUnkown
};
    
struct IPResponeData
{
    std::string _ipAddress;
    std::string _countryCode;
    std::string _countryName;
    std::string _regionCode;
    std::string _regionName;
    std::string _city;
    std::string _zipCode;
    std::string _timeZone;
    double      _latitude;
    double      _longitude;
    int         _metroCode;
    
    IPResponeData()
    {
        _ipAddress = "";
        _countryCode = "";
        _countryName = "";
        _regionCode = "";
        _regionName = "";
        _city = "";
        _zipCode = "";
        _timeZone = "";
        _latitude = 0.0f;
        _longitude = 0.0f;
        _metroCode = 0;
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




















