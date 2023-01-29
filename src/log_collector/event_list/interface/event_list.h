#pragma once

#include "event.h"

#include "deque"

class EventList
{
public:
     EventList( char* data = nullptr, const uint64_t size = 0 );

     bool addEvent( const Event& event );
     bool addEvent( Event&& event );


     uint64_t size() const;
     const Event* getEvent( uint64_t index ) const;

     char* getData( uint64_t* size = nullptr ) const;
     uint64_t getFirstTime() const{ return  firstTime_; };
     uint64_t getLastTime() const{ return endTime_; };
     DataTypes dataType() const{ return type_; };
     bool parseDataAndAdd( char* data, uint64_t size, DataTypes type );

private:
     std::deque<Event> data_;
     uint64_t firstTime_;
     uint64_t endTime_;
     DataTypes type_;
};


