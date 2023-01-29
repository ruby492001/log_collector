#ifndef LOG_COLLECTOR_DB_WRITER_ITF_H
#define LOG_COLLECTOR_DB_WRITER_ITF_H

#include <stdint.h>
#include "event_list.h"
#include <soci/sqlite3/soci-sqlite3.h>
#include <soci/session.h>


enum DatabaseTypes
{
     DTUndefined,
     DTFloat,
     DTDouble,
     DTUint32,
     DTInt32,
     DTUint64,
     DTInt64,
};


class DbWrapperItf
{
public:
     virtual bool initTables() = 0;
     virtual bool connect() = 0;
     virtual bool writeEventsBlobs( const EventList* data_to_write ) = 0;
     virtual bool writeEventsOneRequest( const EventList* data_to_write ) = 0;
     virtual bool writeEventsManyRequests( const EventList* data_to_write ) = 0;
protected:
     virtual void generateValuesString( const EventList* data, std::string& str );
     virtual void generateOneValueString( const Event* ev, std::string& str );
     virtual DatabaseTypes fromDataTypes( DataTypes tp );
};


#endif //LOG_COLLECTOR_DB_WRITER_ITF_H
