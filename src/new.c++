#include "zftdef.h"
#include "memory.h"


void* operator new (size_t length) throw() {
    return Memory::getBlock(length);
}
void operator delete (void* memory) {
    Memory::freeBlock((size_t*)memory);
}
void* operator new[] (size_t count) throw() {
    return Memory::getBlock(count);
}
void operator delete[] (void* memory) {
    Memory::freeBlock((size_t*)memory);
}