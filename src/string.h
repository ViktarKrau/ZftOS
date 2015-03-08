#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* string);
uint8_t strcmp(const char* string_a, const char* string_b);
char* strcpy(char* destination, const char* source);
char* sprintf(char* destination, const char* format, ...);
char* vsprintf(char* destination, const char* format, void* args);
//unsafe
char* int_to_string(int64_t value, char* buffer, uint8_t base);
char* uint_to_string(uint64_t value, char* buffer, uint8_t base);
char* reverse(char* string);