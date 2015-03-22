#include "zftdef.h"



void* operator new(size_t length){
    return (void*)0;
}
void operator delete(void* memory){
    return;
}