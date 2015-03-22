#include "memoryallocator.h"

void* MemoryAllocator::start;

void MemoryAllocator::setStartOffset(uint32_t offset){
    start = (void*)offset;
}

void MemoryAllocator::init() {

}
