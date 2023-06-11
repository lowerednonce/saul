#pragma once
#include <cstdint>

#ifndef __MEMORY_TRACK__

#define __MEMORY_TRACK__

struct AllocationMetrics{
	uint32_t totalAllocated = 0;
	uint32_t totalFreed = 0;

	uint32_t current_usage(){ return totalAllocated-totalFreed; }
};

#endif
