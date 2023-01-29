#include <iostream>

#include "event.h"
#include "event_list.h"


EventList::EventList( char* data, const uint64_t size )
{
     firstTime_ = UINT64_MAX;
     endTime_ = 0;
     type_ = Undefined;
}


bool EventList::addEvent( const Event& event )
{
     if( type_ != Undefined && event.type() != type_ )
     {
          return false;
     }
     if( event.type() == Undefined )
     {
          return false;
     }
     if( type_ == Undefined )
     {
          type_ = event.type();
     }
     if( firstTime_ > event.time() )
     {
          firstTime_ = event.time();
     }
     if( endTime_ < event.time() )
     {
          endTime_ = event.time();
     }
     data_.push_back( event );
     return true;
}


bool EventList::addEvent( Event&& event )
{
     if( type_ != Undefined && event.type() != type_ )
     {
          return false;
     }
     if( event.type() == Undefined )
     {
          return false;
     }
     if( type_ == Undefined )
     {
          type_ = event.type();
     }
     if( firstTime_ > event.time() )
     {
          firstTime_ = event.time();
     }
     if( endTime_ < event.time() )
     {
          endTime_ = event.time();
     }
     data_.push_back( std::move( event ) );
     return true;
}


uint64_t EventList::size() const
{
     return data_.size();
}


const Event* EventList::getEvent( uint64_t index ) const
{
     return &data_[ index ];
}


char* EventList::getData( uint64_t* size ) const
{
     *size = 0;
     if( data_.empty() || type_ == Undefined )
     {
          return nullptr;
     }
     char* result = nullptr;
     if( type_ != dData )
     {
          // если тип константный - можем сразу аллокнуть массив
          *size = data_.begin()->binarySize() * data_.size();
          result = new char[  *size ];

     }
     else
     {
          for( auto& current : data_ )
          {
               *size += current.binarySize();
          }
          if( *size )
          {
               result = new char[ *size ];
          }
     }
     if( !result )
     {
          return result;
     }
     char* currentPos = result;
     for( auto& current: data_)
     {
          currentPos += current.binaryWrite( currentPos );
     }
     return result;
}


bool EventList::parseDataAndAdd( char* data, uint64_t size, DataTypes type )
{
     uint32_t readSize = 0;
     while( true )
     {
          Event tmp( type, data, size, readSize );
          if( !readSize )
          {
               //чтение не удалось
               return false;
          }
          size -= readSize;
          if( !addEvent( std::move( tmp ) ) )
          {
               return false;
          }
          if( !size )
          {
               break;
          }
     }
     std::cout << "Parse data count: " << data_.size() << std::endl;
     return true;
}


