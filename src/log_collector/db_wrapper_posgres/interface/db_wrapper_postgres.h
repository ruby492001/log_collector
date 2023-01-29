#pragma once
#include "db_wrapper_itf.h"

#include <string>
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>


class DbWrapperPostgres: public DbWrapperItf
{
public:
     DbWrapperPostgres( const std::string& databaseName, const std::string& user = "root", const std::string& pass = "root" );
     ~DbWrapperPostgres(){ current_session_.close(); }
     virtual bool initTables() override;
     virtual bool connect() override;
     virtual bool writeEventsBlobs( const EventList* dataToWrite ) override;
     virtual bool writeEventsOneRequest( const EventList* dataToWrite ) override;
     virtual bool writeEventsManyRequests( const EventList* dataToWrite ) override;

private:
     void tableInit( DatabaseTypes type );
     char hexToBin( unsigned char c);

private:
     const std::string dbName_;
     const std::string user_;
     const std::string pass_;
     soci::session current_session_;

private:
     const std::map< DatabaseTypes, std::string > DtToType_ =
             {
                     { DTFloat, "real" },
                     { DTDouble, "double precision"},
                     { DTInt32, "integer" },
                     { DTUint32, "bigint" },
                     { DTInt64, "bigint" },
             };
     const std::map< DatabaseTypes, std::string > DtToName_ =
             {
                     { DTFloat, "TEST_FLOAT" },
                     { DTDouble, "TEST_DOUBLE"},
                     { DTInt32, "TEST_INT" },
                     { DTUint32, "TEST_INT_UNSIGNED" },
                     { DTInt64, "TEST_BIGINT" },
             };
};

