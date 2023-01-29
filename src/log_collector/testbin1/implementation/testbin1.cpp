#include <iostream>
#include "event_list.h"
#include "event_generator.h"
#include "db_wrapper_sqlite.h"
#include "db_wrapper_mysql.h"
#include "db_wrapper_postgres.h"


std::vector< std::shared_ptr< DbWrapperItf > > listDb;
std::vector< std::pair< EventList, std::string > > lists;
uint64_t elementCount = 1000;


bool initDb()
{
     for( auto currentDb: listDb )
     {
          currentDb.get();
          if( !currentDb->connect() )
          {
               return false;
          }
          if( !currentDb->initTables() )
          {
               return false;
          }
     }
     return true;
}


bool testList( const EventList* list, const std::string& format )
{
     std::cout << "Запись данных в формате: " << format << std::endl;
     std::cout << "\tЗапись в базу данных в двоичном виде: " << std::endl;
     for( const auto& currentDb: listDb )
     {
          if( !currentDb->writeEventsBlobs( list ) )
          {
               return false;
          }
     }

     std::cout << "\tЗапись в базу данных одним запросом к базе данных: " << std::endl;
     for( const auto& currentDb: listDb )
     {
          if( !currentDb->writeEventsOneRequest( list ) )
          {
               return false;
          }
     }
     std::cout << "\tЗапись в базу данных несколькими запросами: " << std::endl;
     for( const auto& currentDb: listDb )
     {
          if( !currentDb->writeEventsManyRequests( list ) )
          {
               return false;
          }
     }
     return true;
}


void initLists()
{
     EventList uint64List, int64List, uint32List, int32List, doubleList, floatList;
     EventGenerator gen;

     gen.setList( &uint64List );
     gen.generateUint64( elementCount );
     lists.emplace_back( std::move( uint64List ), "Беззнаковое 64-разрядное число" );

     gen.setList( &int64List );
     gen.generateInt64( elementCount );
     lists.emplace_back( std::move( int64List ), "Знаковое 64-разрядное число" );

     gen.setList( &uint32List );
     gen.generateUint32( elementCount );
     lists.emplace_back( std::move( uint32List ), "Беззнаковое 32-разрядное число" );

     gen.setList( &int32List );
     gen.generateInt32( elementCount );
     lists.emplace_back( std::move( int32List ), "Знаковое 32-разрядное число" );

     gen.setList( &floatList );
     gen.generateFloat( elementCount );
     lists.emplace_back( std::move( floatList ), "Число типа float" );

     gen.setList( &doubleList );
     gen.generateDouble( elementCount );
     lists.emplace_back( std::move( doubleList ), "Число типа double" );
}


int main()
{

     listDb.push_back( std::shared_ptr<DbWrapperItf>( new DbWrapperSqlite( "local_fine.db" ) ) );
     listDb.push_back( std::shared_ptr<DbWrapperItf>( new DbWrapperPostgres( "test" ) ) );
     listDb.push_back( std::shared_ptr<DbWrapperItf>( new DbWrapperMySql( "test" ) ) );
     if( !initDb() )
     {
          std::cout << "Ошибка при инициализации баз данных!" << std::endl;
          return 1;
     }
     std::cout << "Введите число записей, на котором будет проводиться проверка" << std::endl;
     std::cin >> elementCount;
     std::cout << "Тестирование запущено для числа элементов: " << elementCount << std::endl;
     initLists();

     for( const auto& currentList: lists )
     {
          testList( &currentList.first, currentList.second );
     }

     return 0;
}

