//
//  TigerSqliteUtil.cpp
//  AVAD
//
//  Created by Wang zhen on 3/27/15.
//
//

#include "TigerSqliteUtil.h"
#include "TigerMacros.h"
#include "cocos2d.h"


USING_NS_CC;

TigerSqliteUtil* TigerSqliteUtil::_instance = nullptr;

TigerSqliteUtil::TigerSqliteUtil():
m_sqlite(nullptr)
{

}

TigerSqliteUtil::~TigerSqliteUtil()
{

}

TigerSqliteUtil* TigerSqliteUtil::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new (std::nothrow)TigerSqliteUtil();
        
        // 检查db文件是否在可读写目录内，不是的话，复制移动db文件
        if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath()+DB_FILE_NAME))
        {
            _instance->copyDateFileToWriteablePath(FileUtils::getInstance()->fullPathForFilename(DB_FILE_RES_PATH),
                                                   DB_FILE_NAME);
        }
    }
    
    return _instance;
}

void TigerSqliteUtil::destoryInstance()
{
    CC_SAFE_DELETE(_instance);
}

bool TigerSqliteUtil::openDatabaseWithFile(const std::string& dateFile)
{
    int result = sqlite3_open(dateFile.c_str(), &m_sqlite);
    
    if (result != SQLITE_OK)
    {
        TLog("open database failed, number %d", result);
        TLog("db path: %s", dateFile.c_str());
        
        return false;
    }
    
    TLog("### open db is scueesful : %s###", dateFile.c_str());
    
    return true;
}

void TigerSqliteUtil::closeDataBase()
{
    TLog("### close db is scueesful ###");
    
    sqlite3_close(m_sqlite);
    
//    destoryInstance();
}

void TigerSqliteUtil::freeTableWithResult(char **result)
{
    sqlite3_free_table(result);
}

SqliteQueryData TigerSqliteUtil::queryWithSQL(const std::string& sql)
{
    char **result;
    int  r, c;
    
    int error_code;
    char* error_msg = nullptr;
    error_code = sqlite3_get_table(m_sqlite, sql.c_str(), &result, &r, &c, &error_msg);
    
    if (SQLITE_OK != error_code)
    {
        TLog("query is failed[%s]", error_msg);
    }
    
    SqliteQueryData query_data;
    query_data.re = result;
    query_data.r  = r;
    query_data.c  = c;
    
    TLog("query data r[%d], c[%d]", query_data.r, query_data.c);
    
    return query_data;
}

void TigerSqliteUtil::updateWithSQL(const std::string &sql)
{
    int error_code;
    char* error_msg;
    
    error_code = sqlite3_exec(m_sqlite, sql.c_str(), nullptr, nullptr, &error_msg);
    
    if (error_code != SQLITE_OK)
    {
        TLog("update failed[%s]", error_msg);
    }
}

void TigerSqliteUtil::deleteData(const std::string &sql)
{
    int error_code;
    char* error_msg;
    
    error_code = sqlite3_exec(m_sqlite, sql.c_str(), nullptr, nullptr, &error_msg);
    
    if (error_code != SQLITE_OK)
    {
        TLog("delete failed[%s]", error_msg);
    }
}

/**
 In android, the data file must in writeable path. If not, can open the data file.
 @parma filePath ex:"asset/xxx.sqlite"
 @parma fileName ex:"xxx.sqlite"
 */
void TigerSqliteUtil::copyDateFileToWriteablePath(const std::string& filePath, const std::string& fileName)
{
    std::string db_in_writeable = cocos2d::StringUtils::format("%s%s",
                                                               cocos2d::FileUtils::getInstance()->getWritablePath().c_str(),
                                                               fileName.c_str());
    
    TLog("db_in_writeable path: %s", db_in_writeable.c_str());
    
    // Check is file exist in writeable path.
    if (!cocos2d::FileUtils::getInstance()->isFileExist(db_in_writeable))
    {
        TLog("-- The DB file is not exist in writeable path --");
        
        // Check is file exist in resources folder.
        CCASSERT(filePath.c_str(), "");
        
        // Create file in writeable path.
        FILE* out_file = fopen(db_in_writeable.c_str(), "r");
        if (out_file == nullptr)
        {
            // Get data from DB file.
            ssize_t size;
            const char* data = (char*)cocos2d::FileUtils::getInstance()->getFileData(filePath.c_str(),
                                                                                     "rb",
                                                                                     &size);
            
            // Ready wirte data into the out file.
            out_file = fopen(db_in_writeable.c_str(), "wb");
            
            // Write data into file.
            fwrite(data, size, 1, out_file);
            
            CC_SAFE_DELETE_ARRAY(data);
        }
        
        fclose(out_file);
        
        TLog("-- The DB file[%s] copy into writeable path --", fileName.c_str());
    }
    
}

void TigerSqliteUtil::createDataTable(const std::string &dataFile, const std::string &sql)
{
    std::string path = FileUtils::getInstance()->getWritablePath() + dataFile;
    
    m_sqlite = nullptr;
    
    // open a db, if file not exist, then create new one.
//    std::string sql;
    int result;
    char* err_msg = nullptr;
    result = sqlite3_open(path.c_str(), &m_sqlite);
    
    if (result != SQLITE_OK)
    {
        TLog("open database failed, number%d", result);
    }
    
//    std::string sql1 = "CREATE TABLE Plane (ID INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , P_PK INTEGER NOT NULL , P_NAME VARCHAR NOT NULL , P_DATE DOUBLE NOT NULL , P_FUEL INTEGER DEFAULT 100, P_PAINT INTEGER DEFAULT 100, P_CLEAN INTEGER DEFAULT 100, P_QUALITY INTEGER DEFAULT 100, P_LASTFIGHTDATE DOUBLE DEFAULT 0, P_HIGHTRECOD DOUBLE DEFAULT 0)";
    
    result = sqlite3_exec(m_sqlite, sql.c_str(), nullptr, nullptr, &err_msg);
    
    if (result != SQLITE_OK)
    {
        TLog("creare table failed[%s]", err_msg);
    }
    
}

void TigerSqliteUtil::insertData(const std::string &sql)
{
    int result;
    char* err_msg = nullptr;
    result = sqlite3_exec(m_sqlite, sql.c_str(), nullptr, nullptr, &err_msg);
    
    if (result != SQLITE_OK)
    {
        TLog("insert data failed[%s]", err_msg);
    }
}


























