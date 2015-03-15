#ifndef ZFTDEF_H
#define ZFTDEF_H
#include <stddef.h>
#include <stdint.h>

#define BOOL uint8_t
#define TRUE 0xFF
#define FALSE 0x00

typedef uint8_t byte_t;
typedef uint8_t* byte_ptr_t;
typedef int* word_ptr_t;


typedef size_t iter_t;
typedef uint32_t word_iter_t;


#endif /*ZFTDEF_H*/