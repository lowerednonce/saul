#include <memory>
#include <cstdlib>

#include "memory-track.hpp"

extern AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size){
        s_AllocationMetrics.totalAllocated += size;

        return malloc(size);
}

void operator delete(void* memory, size_t size){
        s_AllocationMetrics.totalFreed += size;

        free(memory); 
}
