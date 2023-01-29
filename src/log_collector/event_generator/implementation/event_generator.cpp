#include "event.h"
#include "event_generator.h"
#include <time.h>


EventGenerator::EventGenerator()
:rng_( dev_() )
{
}


void EventGenerator::setList( EventList* list )
{
     list_ = list;
}


uint64_t EventGenerator::generateUint64( uint64_t size, uint64_t minVal, uint64_t maxVal )
{
     if( !list_ )
     {
          return 0;
     }
     std::uniform_int_distribution< uint64_t > dist( minVal,maxVal );
     uint64_t written = 0;
     for( ; written <size; written++ )
     {
          Event ev;
          ev.fromUint64( dist( rng_ ) );
          ev.setTime( time( nullptr ) );
          list_->addEvent( std::move( ev ) );
     }
     return written;
}


uint64_t EventGenerator::generateDouble( uint64_t size, double minVal, double maxVal )
{
     if( !list_ )
     {
          return 0;
     }
     std::uniform_int_distribution< uint64_t > dist( 0,UINT64_MAX );
     uint64_t written = 0;
     for( ; written <size; written++ )
     {
          Event ev;
          ev.fromDouble( minVal + static_cast< double > ( dist( rng_ ) ) /( static_cast < double > ( UINT64_MAX / ( maxVal - minVal ) ) ) );
          ev.setTime( time( nullptr ) );
          list_->addEvent( std::move( ev ) );
     }
     return written;
}


uint64_t EventGenerator::generateFloat( uint64_t size, float minVal, float maxVal )
{
     if( !list_ )
     {
          return 0;
     }
     std::uniform_int_distribution< uint64_t > dist( 0,UINT64_MAX );
     uint64_t written = 0;
     for( ; written <size; written++ )
     {
          Event ev;
          ev.fromFloat( minVal + static_cast< float > ( dist( rng_ ) ) /( static_cast < float > ( UINT64_MAX / ( maxVal - minVal ) ) ) );
          ev.setTime( time( nullptr ) );
          list_->addEvent( std::move( ev ) );
     }
     return written;
}


uint64_t EventGenerator::generateInt64( uint64_t size, int64_t minVal, int64_t maxVal )
{
     if( !list_ )
     {
          return 0;
     }
     std::uniform_int_distribution< int64_t > dist( minVal,maxVal );
     uint64_t written = 0;
     for( ; written <size; written++ )
     {
          Event ev;
          ev.fromInt64( dist( rng_ ) );
          ev.setTime( time( nullptr ) );
          list_->addEvent( std::move( ev ) );
     }
     return written;
}


uint64_t EventGenerator::generateInt32( uint64_t size, int32_t minVal, int32_t maxVal )
{
     if( !list_ )
     {
          return 0;
     }
     std::uniform_int_distribution< int32_t > dist( minVal,maxVal );
     uint64_t written = 0;
     for( ; written <size; written++ )
     {
          Event ev;
          ev.fromInt32( dist( rng_ ) );
          ev.setTime( time( nullptr ) );
          list_->addEvent( std::move( ev ) );
     }
     return written;
}


uint64_t EventGenerator::generateUint32( uint64_t size, int32_t minVal, int32_t maxVal )
{
     if( !list_ )
     {
          return 0;
     }
     std::uniform_int_distribution< uint32_t > dist( minVal,maxVal );
     uint64_t written = 0;
     for( ; written <size; written++ )
     {
          Event ev;
          ev.fromUInt32( dist( rng_ ) );
          ev.setTime( time( nullptr ) );
          list_->addEvent( std::move( ev ) );
     }
     return written;
}
