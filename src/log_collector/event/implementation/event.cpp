#include "event.h"


#include <cstring>


Event::Event( uint64_t time, DataTypes type, char* data, uint32_t dataSize )
: time_( time ), type_( type ), data_( data ), dataSize_( dataSize )
{
}


void Event::fromInt64( int64_t val )
{
     type_ = dInt64;
     saveData( (char*)&val, sizeof( val ) );
}


void Event::fromUint64( uint64_t val )
{
     type_ = dUInt64;
     saveData( (char*)&val, sizeof( val ) );
}


void Event::fromInt32( int32_t val )
{
     type_ = dInt32;
     saveData( (char*)&val, sizeof( val ) );
}


void Event::fromUInt32( uint32_t val )
{
     type_ = dUInt32;
     saveData( (char*)&val, sizeof( val ) );
}


void Event::fromFloat( float val )
{
     type_ = dFloat;
     saveData( (char*)&val, sizeof( val ) );
}


void Event::fromDouble( double val )
{
     type_ = dDouble;
     saveData( (char*)&val, sizeof( val ) );
}


void Event::setTime( uint64_t time )
{
     time_ = time;
}


void Event::setData( char* data, uint32_t dataSize )
{
     type_ = dData;
     saveData( data, dataSize );
}


DataTypes Event::type() const
{
     return type_;
}


uint64_t Event::time() const
{
     return time_;
}


int64_t Event::toInt64() const
{
     return *( int64_t* )( data_ );
}


uint64_t Event::toUint64() const
{
     return *( uint64_t* )( data_ );
}


int32_t Event::toInt32() const
{
     return *( int32_t* )( data_ );
}


uint32_t Event::toUInt32() const
{
     return *( uint32_t* )( data_ );
}


float Event::toFloat() const
{
     return *( float* )( data_ );
}


double Event::toDouble() const
{
     return *( double* )( data_ );
}


const char* Event::toData() const
{
     return data_;
}


uint32_t Event::dataSize() const
{
     return dataSize_;
}


Event::Event( const Event& rhs )
{
     *this = rhs;
}


Event::Event( Event&& rhs )
{
     data_ = rhs.data_;
     rhs.data_ = nullptr;
     type_ = rhs.type_;
     rhs.type_ = Undefined;
     dataSize_ = rhs.dataSize_;
     rhs.dataSize_ = 0;
     time_ = rhs.time_;
}


Event& Event::operator=( const Event& rhs )
{
     if( data_ )
     {
          delete[] data_;
     }
     data_ = new char[ rhs.dataSize_ ];
     memcpy( data_, rhs.data_, rhs.dataSize_ );
     dataSize_ = rhs.dataSize_;
     time_ = rhs.time_;
     type_ = rhs.type_;
     return *this;
}


Event::~Event()
{
     if( data_ )
     {
          delete data_;
     }
}


void Event::saveData( char* data, uint32_t dataSize )
{
     if( data_ )
     {
          delete[] data_;
     }
     data_ = new char[ dataSize ];
     memcpy( data_, data, dataSize );
     dataSize_ = dataSize;
}


uint64_t Event::binarySize() const
{
     if( type_ == Undefined )
     {
          return 0;
     }
     if( type_ != dData )
     {
          return sizeof( time_ ) + dataSize_;
     }
     return sizeof( time_ ) + dataSize_ + sizeof( dataSize_ );
}


uint64_t Event::binaryWrite( char* ptr ) const
{
     if( type_ == Undefined )
     {
          return 0;
     }
     memcpy( ptr, &time_, sizeof( time_ ) );
     ptr += sizeof( time_ );
     if( type_ == dData )
     {
          memcpy( ptr, &dataSize_, sizeof( dataSize_ ) );
          ptr += sizeof( dataSize_ );
     }
     memcpy( ptr, data_, dataSize_ );
     return binarySize();
}


uint32_t Event::fromBinary( char* binary, uint32_t avalSize, DataTypes type )
{
     uint32_t result = 0;
     type_ = type;
     if( avalSize < sizeof( time_ ) )
     {
          return 0;
     }
     memcpy( &time_, binary, sizeof( time_ ) );
     result += sizeof( time_ );
     avalSize -= sizeof( time_ );
     if( type == dData )
     {
          if( avalSize < sizeof( dataSize_ ) )
          {
               return 0;
          }
          memcpy( &dataSize_, binary + result, sizeof( dataSize_ ) );
          result += sizeof( dataSize_ );
          avalSize -= sizeof( dataSize_ );
     }
     else
     {
          dataSize_ = sizeDataTypes( type );
     }
     data_ = new char[ dataSize_ ];
     if( avalSize < dataSize_ )
     {
          delete[] data_;
          data_ = nullptr;
          return 0;
     }
     memcpy( data_, binary + result, dataSize_ );
     return result + dataSize_;
}


Event::Event( DataTypes type, char* data, const uint32_t& avalSize, uint32_t& size )
:Event()
{
     size = fromBinary( data, avalSize, type );
}


uint32_t Event::sizeDataTypes( DataTypes types )
{
     switch( types )
     {
          case dUInt64:
          case dInt64:
          {
               return sizeof( uint64_t );
          }
          case dUInt32:
          case dInt32:
          {
               return sizeof( uint32_t );
          }
          case dFloat:
          {
               return sizeof( float );
          }
          case dDouble:
          {
               return sizeof( double );
          }
     }
     return 0;
}
