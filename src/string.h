#include "zftdef.h"

char* strcpy(char* destination, const char* source);
uint8_t strcmp(const char* string_a, const char* string_b);
size_t strlen(const char* string);
BOOL is_empty(const char* string);
//char* sprintf(char* destination, const char* format, ...);
//char* vsprintf(char* destination, const char* format, void* args);
//unsafe
char* int_to_string(int64_t value, char* buffer, uint8_t base);
char* uint_to_string(uint64_t value, char* buffer, uint8_t base);
double string_to_float(char* source, uint8_t base);
char* float_to_string(double value, char* buffer, uint8_t base);
int64_t string_to_int(char* source, uint8_t base);

char* reverse(char* string);
