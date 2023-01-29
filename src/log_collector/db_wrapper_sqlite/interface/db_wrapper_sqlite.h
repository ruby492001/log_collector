#ifndef LOG_COLLECTOR_DB_WRAPPER_SQLITE_H
#define LOG_COLLECTOR_DB_WRAPPER_SQLITE_H


#include "db_wrapper_itf.h"

#include <string>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>






class DbWrapperSqlite: public DbWrapperItf
{
public:
     DbWrapperSqlite( const std::string& pathToFile );
     ~DbWrapperSqlite(){ current_session_.close(); }
     virtual bool initTables() override;
     virtual bool connect() override;
     virtual bool writeEventsBlobs( const EventList* dataToWrite ) override;
     virtual bool writeEventsOneRequest( const EventList* dataToWrite ) override;
     virtual bool writeEventsManyRequests( const EventList* dataToWrite ) override;

private:
     void tableInit( DatabaseTypes type );

private:
     const std::string pathToFile_;
     soci::session current_session_;

private:
     const std::map< DatabaseTypes, std::string > DtToType_ =
             {
                     { DTFloat, "REAL" },
                     { DTDouble, "REAL"},
                     { DTInt32, "INTEGER" },
                     { DTInt64, "INTEGER"},
                     { DTUint32, "UNSIGNED INTEGER" },
                     { DTUint64, "UNSIGNED INTEGER" },
             };
};


#endif
