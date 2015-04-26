#include "../zftdef.h"
#include "memory.h"



void* operator new (size_t length) throw() {
    return Memory::getBlock(length);
}



void* operator new (size_t length __attribute__((unused)), void* startAddress) {
    return startAddress;
}



void* operator new[] (size_t length __attribute__((unused)), void* startAddress) {
    return startAddress;
}



void operator delete (void* memory) {
    if (memory != nullptr) {
        Memory::freeBlock((size_t*)memory);
    }
}



void* operator new[] (size_t count) throw() {
    return Memory::getBlock(count);
}



void operator delete[] (void* memory) {
    if (memory != nullptr) {
        Memory::freeBlock((size_t*) memory);
    }
}
