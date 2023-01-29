#pragma once

#include <random>
#include "event_list.h"
#include <limits>


class EventGenerator
{
public:
     EventGenerator();
     void setList( EventList* list );
     uint64_t generateUint64( uint64_t size, uint64_t minVal = 0, uint64_t maxVal = UINT64_MAX  );
     uint64_t generateDouble( uint64_t size, double minVal = std::numeric_limits<double>::min(), double maxVal = std::numeric_limits<double>::max() );
     uint64_t generateFloat( uint64_t size, float minVal = std::numeric_limits<float>::min(), float maxVal = std::numeric_limits<float>::max());
     uint64_t generateInt64( uint64_t size, int64_t minVal = 0, int64_t maxVal = INT64_MAX );
     uint64_t generateInt32( uint64_t size, int32_t minVal = -INT32_MAX, int32_t maxVal = INT32_MAX );
     uint64_t generateUint32( uint64_t size, int32_t minVal = 0, int32_t maxVal = UINT32_MAX );
private:
     std::random_device dev_;
     std::mt19937 rng_;
     EventList* list_ = nullptr;
};

