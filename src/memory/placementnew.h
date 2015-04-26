#include "../zftdef.h"



#ifndef ZFTOS_DEV_PLACEMENTNEW_H
#define ZFTOS_DEV_PLACEMENTNEW_H



void* operator new (size_t, void*) throw();
void* operator new[] (size_t, void*) throw();


#endif
