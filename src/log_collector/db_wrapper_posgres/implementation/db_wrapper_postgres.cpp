#include "db_wrapper_postgres.h"

#include <exception>
#include <iostream>
#include "profiler.h"


DbWrapperPostgres::DbWrapperPostgres( const std::string& databaseName, const std::string& user, const std::string& pass )
:dbName_( databaseName ), user_( user ), pass_( pass )
{
}


bool DbWrapperPostgres::connect()
{
     try
     {
          current_session_.open( soci::postgresql, "dbname=" + dbName_ + " user=" + user_ + " password=" + pass_ );
          return current_session_.is_connected();
     }
     catch( soci::soci_error& er)
     {
          std::cerr << "Connect error:" << er.what();
     }
     return false;
}


bool DbWrapperPostgres::initTables()
{
     try
     {
          for( const auto& cur: DtToType_ )
          {
               tableInit( cur.first );
          }
          current_session_ << "CREATE TABLE IF NOT EXISTS TEST_BLOB (start_time bigint, end_time bigint, data text);";
     }
     catch( std::exception& ex )
     {
          std::cerr << "SQLITE INIT TABLE EX: " << ex.what() << std::endl;
          return false;
     }
     return true;
}


void DbWrapperPostgres::tableInit( DatabaseTypes type )
{
     current_session_ << "CREATE TABLE IF NOT EXISTS " + DtToName_.at( type ) + "(time bigint, value " +  DtToType_.at( type ) + ");";
}


bool DbWrapperPostgres::writeEventsBlobs( const EventList* dataToWrite )
{
     uint64_t startTime = dataToWrite->getFirstTime();
     uint64_t endTime = dataToWrite->getLastTime();
     uint64_t dataSize;
     std::string tmp_blob;
     std::string blob;
     char* data;
     {
          LOG_DURATION( "\t\tPostgres подготовка данных" );
          data = dataToWrite->getData( &dataSize );
          tmp_blob.append( data, dataSize );
          blob.reserve( tmp_blob.size() * 2 + 2 );
          blob = "\\x";
          for( uint64_t idx = 0; idx < tmp_blob.size(); idx++ )
          {
               blob += hexToBin( ( tmp_blob[idx] & 0x11110000 ) >> 4 ) + hexToBin( tmp_blob[idx] & 0x1111 );
          }
          delete[] data;
     }
     try
     {
          LOG_DURATION( "\t\tPostgres запись данных" );
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


bool DbWrapperPostgres::writeEventsOneRequest( const EventList* dataToWrite )
{
     if( dataToWrite->dataType() == dUInt64 )
     {
          std::cout << "\t\tPostgres не поддерживает беззнаковое 64-разрядное число!" << std::endl;
          return true;
     }
     std::string values;
     try
     {
          {
               LOG_DURATION( "\t\tPostgres подготовка данных" );
               generateValuesString( dataToWrite, values );

          }
          if( values.empty() )
          {
               return false;
          }
          {
               LOG_DURATION( "\t\tPostgres запись данных" );
               current_session_ << "INSERT INTO " + DtToName_.at( fromDataTypes( dataToWrite->dataType() ) ) + "(time, value) " +
                                   "VALUES " + values + ";";
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


bool DbWrapperPostgres::writeEventsManyRequests( const EventList* dataToWrite )
{
     if( dataToWrite->dataType() == dUInt64 )
     {
          std::cout << "\t\tPostgres не поддерживает беззнаковое 64-разрядное число!" << std::endl;
          return true;
     }
     std::string value;
     try
     {
          LOG_DURATION( "\t\tPostgres запись данных" );
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


char DbWrapperPostgres::hexToBin( unsigned char c )
{
     if( c < 10 )
     {
          return '0' + c;
     }
     return 'A' + c - 10;
}




