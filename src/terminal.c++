#include "terminal.h"
#include "iob.h"
#include "zft_memory.h"
#include "string.h"



Terminal::Terminal() {
	setColor(COLOR_WHITE, COLOR_BLACK);
	init();
}



Terminal::Terminal(TerminalColor foreground, TerminalColor background) {
	setColor(foreground, background);
	init();
}



void Terminal::clear() {
	init();
}



void Terminal::redraw() {
	uint8_t* vgaPtr = (uint8_t*)0xB8001;
	const uint8_t* vgaEndPtr = (uint8_t*)(0xB8000 + 80 * 25 * 2);
	while(vgaPtr < vgaEndPtr){
		*vgaPtr = color;
		vgaPtr += 2;
	}
}



ColorByte Terminal::getColor() {
	return color;	
}



void Terminal::init() {
	row = 0;
	column = 0;	
	startPtr = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; ++y){
		for (size_t x = 0; x < VGA_WIDTH; ++x){
			const size_t index = y * VGA_WIDTH + x;
			startPtr[index] = makeColoredChar(0);
		}
	}
}



size_t Terminal::getPosition() {
	return row * column;
}



void Terminal::setColor(ColorScheme scheme) {
	color = scheme.byte;
}



void Terminal::setColor(ColorByte _color) {
	color = _color;
}



void Terminal::setColor(TerminalColor foreground, TerminalColor background) {
	color = makeColor(foreground, background);
}



void Terminal::newLine() {
	column = 0;
	if (row == VGA_HEIGHT - 1){
		scroll();
	}
	else{
		++row;
	}
}



void Terminal::putColoredCharAtPos(char c, ColorByte _color, size_t x, size_t y) {
	*(startPtr + y * VGA_WIDTH + x) = makeColoredChar(c, _color);
}



void Terminal::putchar(char c) {
	if(c == '\n'){
		newLine();
		updateCursor();
		return;
	}
	if(c == '\b'){
		backspace();
		updateCursor();
		return;
	}
	if(c == '\t'){
		puts("    ");
		updateCursor();
		return;
	}
	putCharDefault(c);
	if (++column == VGA_WIDTH){
		newLine();
	}
	updateCursor();
}



void Terminal::backspace() {
	if(column){
		column--;
	}
	else{
		column = VGA_WIDTH - 1;
		row -= 1;
	}
	putCharDefault(0);
}



void Terminal::puts(const char* string) {
	for(size_t i = 0; string[i] != '\0'; ++i){
		putchar(string[i]);
	}
}



void Terminal::putsln(const char* string) {
	puts(string);
	newLine();
}



void Terminal::putint(int64_t value) {
	puts(int_to_string(value, numberBuff, 10));
}



void Terminal::putuint(uint64_t value) {
	puts(uint_to_string(value, numberBuff, 10));
}



void Terminal::puthex(uint64_t value) {
	puts("0x");
	puts(uint_to_string(value, numberBuff, 0x10));
}



Terminal::~Terminal() {
}



void Terminal::putCharDefault(char c) {
	*(startPtr + row * VGA_WIDTH + column) = makeColoredChar(c);
}



ColorByte Terminal::makeColor(TerminalColor foreground, TerminalColor background) {
	return foreground | background << 4;
}



void Terminal::scroll() {
	for(uint8_t i = 0; i < VGA_HEIGHT - 1; ++i){
		memcpy((void*)(startPtr + i * VGA_WIDTH)
			, (void*)(startPtr + (i + 1) * VGA_WIDTH), VGA_WIDTH * 2);
	}
	uint16_t empty_place = makeColoredChar(0);
	memduplicate((void*)(startPtr + (VGA_HEIGHT - 1) * VGA_WIDTH)
		, (void*)&empty_place, 2, VGA_WIDTH); 
}



uint16_t Terminal::makeColoredChar(char c) {
	uint16_t char16 = (uint16_t) c;
	uint16_t color16 = color;
	return char16 | color16 << 8;
}



uint16_t Terminal::makeColoredChar(char c, ColorByte _color) {
	uint16_t char16 = (uint16_t) c;
	uint16_t color16 = _color;
	return char16 | color16 << 8;
}



void Terminal::putbytes(uint64_t value) {
	puthex(value);
	puts(" B  or  ");
	putuint(value >> 10);
	puts(" KB  or  ");
	putuint(value >> 20);
	puts(" MB");
}



void Terminal::updateCursor(){
    uint16_t position = (uint16_t) ((row * 80) + column);
	// cursor LOW port to vga INDEX register
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(position & 0xFF));
	// cursor HIGH port to vga INDEX register
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}