#include "zftdef.h"
#include "memory.h"


void* operator new (size_t length) throw() {
    return forbidden::Memory::getBlock(length);
}
void operator delete (void* memory) {
    forbidden::Memory::freeBlock((size_t*)memory);
}
void* operator new[] (size_t count) throw() {
    return forbidden::Memory::getBlock(count);
}
void operator delete[] (void* memory) {
    forbidden::Memory::freeBlock((size_t*)memory);
}