#include <stddef.h>
#include <stdint.h>
enum terminal_color{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
void kprintf(const char* format, ...);
void terminal_initialize();
void kset_color(enum terminal_color foreground, enum terminal_color background);
void kputchar(char c);
void kputs(const char* string);
void kputsln(const char* string);
void kputint(int64_t value);
void kputuint(uint64_t value);