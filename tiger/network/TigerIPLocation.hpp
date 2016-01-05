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
    China,
    Korea,
    Other
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
    
    bool parseJson(const std::string json);
    
private:
    static TigerIPLocation* _instance;
};

}

#endif /* TigerIPLocation_hpp */




















