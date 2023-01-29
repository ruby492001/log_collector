#include "db_wrapper_mysql.h"

#include <exception>
#include <iostream>
#include "profiler.h"


DbWrapperMySql::DbWrapperMySql( const std::string& databaseName, const std::string& user, const std::string& pass )
:dbName_( databaseName ), user_( user ), pass_( pass )
{
}


bool DbWrapperMySql::connect()
{
     try
     {
          current_session_.open( soci::mysql, "db=" + dbName_ + " user=" + user_ + " password=" + pass_ );
          return current_session_.is_connected();
     }
     catch( soci::soci_error& er)
     {
          std::cerr << "Connect error:" << er.what();
     }
     return false;
}


bool DbWrapperMySql::initTables()
{
     try
     {
          for( const auto& cur: DtToType_ )
          {
               tableInit( cur.first );
          }
          current_session_ << "CREATE TABLE IF NOT EXISTS TEST_BLOB (start_time BIGINT UNSIGNED, end_time BIGINT UNSIGNED, data MEDIUMBLOB);";
     }
     catch( std::exception& ex )
     {
          std::cerr << "SQLITE INIT TABLE EX: " << ex.what() << std::endl;
          return false;
     }
     return true;
}


void DbWrapperMySql::tableInit( DatabaseTypes type )
{
     current_session_ << "CREATE TABLE IF NOT EXISTS " + DtToName_.at( type ) + "(time BIGINT UNSIGNED, value " +  DtToType_.at( type ) + ");";
}


bool DbWrapperMySql::writeEventsBlobs( const EventList* dataToWrite )
{
     uint64_t startTime = dataToWrite->getFirstTime();
     uint64_t endTime = dataToWrite->getLastTime();
     uint64_t dataSize;
     std::string blob;
     char* data;
     {
          LOG_DURATION( "\t\tMySql подготовка данных" );
          data = dataToWrite->getData( &dataSize );
          blob.append( data, dataSize );
          delete[] data;
     }
     try
     {
          LOG_DURATION( "\t\tMySql запись данных" );
          current_session_ << "INSERT INTO TEST_BLOB(start_time, end_time, data) VALUES(:start, :end, :data);",
                  soci::use( startTime, "start" ), soci::use( endTime, "end" ),  soci::use( blob, "data" );
     }
     catch( const std::exception& ex)
     {
          std::cerr << "SQLITE WRITE BLOBS EX: " << ex.what() << std::endl;
          return false;
     }
     return true;
}


bool DbWrapperMySql::writeEventsOneRequest( const EventList* dataToWrite )
{
     std::string values;
     try
     {
          {
               LOG_DURATION( "\t\tMySql подготовка данных" );
               generateValuesString( dataToWrite, values );
          }
          if( values.empty() )
          {
               return false;
          }
          LOG_DURATION( "\t\tMySql запись данных" );
          current_session_ << "INSERT INTO " + DtToName_.at( fromDataTypes( dataToWrite->dataType() ) ) + "(time, value) " +
                              "VALUES " + values + ";";
          return true;
     }
     catch( soci::soci_error& er )
     {
          std::cerr << "SQL error" << er.what();
     }
     catch( std::exception& ex )
     {
          std::cerr << "Internal error: " << ex.what();
     }
     return false;
}


bool DbWrapperMySql::writeEventsManyRequests( const EventList* dataToWrite )
{
     std::string value;
     try
     {
          LOG_DURATION( "\t\tMySql запись данных" );
          for( uint64_t idx = 0; idx < dataToWrite->size(); idx++ )
          {
               generateOneValueString( dataToWrite->getEvent( idx), value );
               current_session_ << "INSERT INTO " + DtToName_.at( fromDataTypes( dataToWrite->dataType() ) ) + "(time, value) " +
                                   "VALUES " + value + ";";
               value.clear();
          }
          return true;
     }
     catch( soci::soci_error& er )
     {
          std::cerr << "SQL error" << er.what();
     }
     catch( std::exception& ex )
     {
          std::cerr << "Internal error: " << ex.what();
     }
     return false;
}




