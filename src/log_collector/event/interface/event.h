#pragma once

#include <cstdint>


enum DataTypes
{
     Undefined,
     dUInt64,
     dUInt32,
     dInt64,
     dInt32,
     dFloat,
     dDouble,
     dData
};


class Event
{
public:
     //функции сохранения данных
     Event( uint64_t time = 0, DataTypes type = Undefined, char* data = nullptr, uint32_t dataSize = 0 );
     Event( DataTypes type, char* data, const uint32_t& avalSize, uint32_t& size );
     Event( const Event& rhs );
     Event( Event&& rhs );
     virtual ~Event();
     Event& operator=( const Event& rhs);
     void fromInt64( int64_t val );
     void fromUint64( uint64_t val );
     void fromInt32( int32_t val );
     void fromUInt32( uint32_t val );
     void fromFloat( float val );
     void fromDouble( double val );
     void setTime( uint64_t time );
     void setData( char* data, uint32_t dataSize );

     //функции получения данных
     DataTypes type() const;
     uint64_t time() const;
     int64_t toInt64() const;
     uint64_t toUint64() const;
     int32_t toInt32() const;
     uint32_t toUInt32() const;
     float toFloat() const;
     double toDouble() const;
     const char* toData() const;
     uint32_t dataSize() const;

     uint64_t binarySize() const;
     uint64_t binaryWrite( char* ptr ) const;

     static uint32_t sizeDataTypes( DataTypes types );
private:
     void saveData( char* data, uint32_t dataSize );
     uint32_t fromBinary( char* binary, uint32_t avalSize, DataTypes type );

private:
     uint64_t time_;
     char* data_;
     uint32_t dataSize_;
     DataTypes type_;
};


