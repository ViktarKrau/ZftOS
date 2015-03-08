#include "zft_stdo.h"
#include "zft_memory.h"
#include "string.h"
#include <stddef.h>
#include <stdint.h>
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
//Hardware text mode color constants. 

typedef uint8_t color_byte;

//Terminal variables 
struct __terminal{
	size_t row;
	size_t column;
	uint8_t color;
	uint16_t* start_pointer;
} terminal;

uint8_t make_color(enum terminal_color foreground, enum terminal_color background){
	return foreground | background << 4;
}


uint16_t make_colored_char(char c, uint8_t color){
	uint16_t char16 = c;
	uint16_t color16 = color;
	return char16 | color16 << 8;
}
void terminal_initialize(){
	terminal.row = 0;
	terminal.column = 0;
	terminal.color = make_color(COLOR_WHITE, COLOR_BLACK);
	terminal.start_pointer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; ++y){
		for (size_t x = 0; x < VGA_WIDTH; ++x){
			const size_t index = y * VGA_WIDTH + x;
			terminal.start_pointer[index] = make_colored_char(' ', terminal.color);
		}
	}
}



void kset_color(enum terminal_color foreground, enum terminal_color background){
	terminal.color = make_color(foreground, background);
}
 
void terminal_put_colored_char_at_pos(char c, uint8_t color, size_t x, size_t y){
	*(terminal.start_pointer + y * VGA_WIDTH + x) = make_colored_char(c, color);
}
void terminal_put_char_default(char c){
	*(terminal.start_pointer + terminal.row * VGA_WIDTH + terminal.column) = make_colored_char(c, terminal.color);
}
void terminal_scroll(){
	for(uint8_t i = 0; i < VGA_HEIGHT - 1; ++i){
		memcpy((void*)(terminal.start_pointer + i * VGA_WIDTH)
			, (void*)(terminal.start_pointer + (i + 1) * VGA_WIDTH), VGA_WIDTH * 2);
	}
	uint16_t empty_place = make_colored_char(' ', terminal.color);
	memduplicate((void*)(terminal.start_pointer + (VGA_HEIGHT - 1) * VGA_WIDTH)
		, (void*)&empty_place, 2, VGA_WIDTH); 

}
void terminal_newline(){
	terminal.column = 0;
	if (terminal.row == VGA_HEIGHT - 1){
		terminal_scroll();
	}
	else{
		++terminal.row;
	}
}
void kputchar(char c){
	if(c == '\n'){
		terminal_newline();
		return;
	}
	terminal_put_char_default(c);
	if (++terminal.column == VGA_WIDTH){
		terminal_newline();
	}
}
 
void kputs(const char* string){
	for(size_t i = 0; string[i] != '\0'; ++i){
		kputchar(string[i]);
	}
}
void kputsln(const char* string){
	kputs(string);
	terminal_newline();
}

void kprintf(const char* format, ...){
	char* symbol_ptr = (char*)format;
	while(*symbol_ptr){
		if(*symbol_ptr == '%'){
			switch(*(symbol_ptr + 1)){

			}
		}
	}
}
void kputint(int64_t value){
	char buff[8];
	kputs(int_to_string(value, buff, 10));
}
void kputuint(uint64_t value){
	char buff[8];
	kputs(uint_to_string(value, buff, 10));
}
