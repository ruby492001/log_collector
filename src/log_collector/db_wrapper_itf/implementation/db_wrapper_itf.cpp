#include "../interface/db_wrapper_itf.h"


void DbWrapperItf::generateValuesString( const EventList* data, std::string& str )
{
     str.clear();
     str.reserve( data->size() * ( sizeof( uint64_t ) * 2 ) );
     for( int idx = 0; idx < data->size(); idx++ )
     {
          generateOneValueString( data->getEvent( idx), str );
          str += ",";
     }
     // удаляем последнюю запятую
     str.pop_back();
}


DatabaseTypes DbWrapperItf::fromDataTypes( DataTypes tp)
{
     switch( tp )
     {
          case dUInt64:
          {
               return DTUint64;
          }
          case dUInt32:
          {
               return DTUint32;
          }
          case dInt64:
          {
               return DTInt64;
          }
          case dInt32:
          {
               return DTInt32;
          }
          case dFloat:
          {
               return DTFloat;
          }
          case dDouble:
          {
               return DTDouble;
          }
          default:
               throw std::logic_error( "Not supported type" );
     }
}


void DbWrapperItf::generateOneValueString( const Event* currentEvent, std::string& str )
{
     switch( currentEvent->type() )
     {
          case dUInt64:
          {
               str += "(" + std::to_string( currentEvent->time() ) + " , " + std::to_string( currentEvent->toUint64() ) + ")";
               break;
          }
          case dInt64:
          {
               str += "(" + std::to_string( currentEvent->time() ) + " , " + std::to_string( currentEvent->toInt64() ) + ")";
               break;
          }
          case dDouble:
          {
               str += "(" + std::to_string( currentEvent->time() ) + " , " + std::to_string( currentEvent->toDouble() ) + ")";
               break;
          }
          case dFloat:
          {
               str += "(" + std::to_string( currentEvent->time() ) + " , " + std::to_string( currentEvent->toFloat() ) + ")";
               break;
          }
          case dUInt32:
          {
               str += "(" + std::to_string( currentEvent->time() ) + " , " + std::to_string( currentEvent->toUInt32() ) + ")";
               break;
          }
          case dInt32:
          {
               str += "(" + std::to_string( currentEvent->time() ) + " , " + std::to_string( currentEvent->toInt32() ) + ")";
               break;
          }
          default:
          {
               throw std::logic_error( "Non support type" );
          }
     }
}
