#include "db_wrapper_sqlite.h"

#include <exception>
#include <iostream>
#include "profiler.h"


DbWrapperSqlite::DbWrapperSqlite( const std::string& pathToFile )
:pathToFile_( pathToFile )
{
}


bool DbWrapperSqlite::connect()
{
     current_session_.open( soci::sqlite3, pathToFile_ );
     return current_session_.is_connected();
}


bool DbWrapperSqlite::initTables()
{
     try
     {
          for( const auto& cur: DtToType_ )
          {
               tableInit( cur.first );
          }
          current_session_ << "CREATE TABLE IF NOT EXISTS TEST_BLOB (start_time LARGE INTEGER, end_time LARGE INTEGER, data BLOB);";
     }
     catch( std::exception& ex )
     {
          std::cout << "SQLITE INIT TABLE EX: " << ex.what();
          return false;
     }
     return true;
}


void DbWrapperSqlite::tableInit( DatabaseTypes type )
{
     current_session_ << "CREATE TABLE IF NOT EXISTS \"TEST_" + DtToType_.at( type ) + "\" (time LARGE INTEGER, value " +  DtToType_.at( type ) + ");";
}


bool DbWrapperSqlite::writeEventsBlobs( const EventList* dataToWrite )
{
     uint64_t startTime = dataToWrite->getFirstTime();
     uint64_t endTime = dataToWrite->getLastTime();
     uint64_t dataSize;
     soci::blob blob( current_session_ );
     char* data;
     {
          LOG_DURATION( "\t\tSQlite подготовка данных" );
          data = dataToWrite->getData( &dataSize );
          blob.append(  data, dataSize );
          delete[] data;
     }

     try
     {
          LOG_DURATION( "\t\tSQlite запись данных" );
          current_session_ << "INSERT INTO TEST_BLOB(start_time, end_time, data) VALUES(:start, :end, :data);",
                  soci::use( startTime, "start" ), soci::use( endTime, "end" ),  soci::use( blob, "data" );
     }
     catch( const std::exception& ex)
     {
          std::cout << "SQLITE WRITE BLOBS EX: " << ex.what();
          return false;
     }
     return true;
}


bool DbWrapperSqlite::writeEventsOneRequest( const EventList* dataToWrite )
{
     std::string values;
     try
     {
          {
               LOG_DURATION( "\t\tSQlite подготовка данных" );
               generateValuesString( dataToWrite, values );
          }
          if( values.empty() )
          {
               return false;
          }
          LOG_DURATION( "\t\tSQlite запись данных" );
          current_session_ << "INSERT INTO \"TEST_" + DtToType_.at( fromDataTypes( dataToWrite->dataType() ) ) + "\"(time, value) " +
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


bool DbWrapperSqlite::writeEventsManyRequests( const EventList* dataToWrite )
{
     std::string value;
     try
     {
          LOG_DURATION( "\t\tSQlite запись данных" );
          for( uint64_t idx = 0; idx < dataToWrite->size(); idx++ )
          {
               generateOneValueString( dataToWrite->getEvent( idx), value );
               current_session_ << "INSERT INTO \"TEST_" + DtToType_.at( fromDataTypes( dataToWrite->dataType() ) ) + "\"(time, value) " +
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


