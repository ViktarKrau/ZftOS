#include <stdint-gcc.h>
#include "zftdef.h"
#ifndef _ZFTOS_DEV_MEMORYALLOCATOR_H_
#define _ZFTOS_DEV_MEMORYALLOCATOR_H_


class MemoryAllocator {
public:
    static void setStartOffset(uint32_t offset);
    static void init();
private:
    static void* start;


};


#endif //_ZFTOS_DEV_MEMORYALLOCATOR_H_
