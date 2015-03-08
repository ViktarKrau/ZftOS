#include <stddef.h>
#include <stdint.h>

void memcpy(void* destination, const void* source, size_t length);
void memduplicate(void* destination, const void* source, size_t source_length, size_t times);