//
//  TigerSqliteUtil.h
//  AVAD
//
//  Created by Wang zhen on 3/27/15.
//
//

#ifndef __AVAD__TigerSqliteUtil__
#define __AVAD__TigerSqliteUtil__

#include "sqlite3.h"
#include <string>

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <unordered_map>
#include <vector>
#endif

#define DB_FILE_NAME            "testFF.sqlite"
#define DB_FILE_RES_PATH        "db/testFF.sqlite"

#define DB_PATH FileUtils::getInstance()->getWritablePath() + DB_FILE_NAME

struct SqliteQueryData
{
    char **re;
    int  r;
    int  c;
};

class TigerSqliteUtil
{
public:
    
    TigerSqliteUtil();
    ~TigerSqliteUtil();
    
    static TigerSqliteUtil* getInstance();
    static void destoryInstance();
    
    void createDataTable(const std::string& dataFile, const std::string& sql);
    bool openDatabaseWithFile(const std::string& dateFile);
    void freeTableWithResult(char** result);
    void closeDataBase();
    
    SqliteQueryData queryWithSQL(const std::string& sql);
    
    void updateWithSQL(const std::string& sql);
    
    void copyDateFileToWriteablePath(const std::string& filePath, const std::string& fileName);
    
    void insertData(const std::string& sql);
    
    void deleteData(const std::string& sql);
    
private:
    
    sqlite3* m_sqlite;
    
    static TigerSqliteUtil *_instance;
};


#endif /* defined(__AVAD__TigerSqliteUtil__) */
