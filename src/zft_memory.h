#include "zftdef.h"
void* memzero(void* destination, size_t length);
void* memcpy(void* destination, const void* source, size_t length);
void* memduplicate(void* destination, const void* source, size_t source_length, size_t times);