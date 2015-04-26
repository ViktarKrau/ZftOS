#include "../zftdef.h"



#define VGA_START_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 24
#define STATUS_START_ADDRESS (VGA_START_ADDRESS + VGA_WIDTH * VGA_HEIGHT * 2)
#define STATUS_END_ADDRESS (VGA_START_ADDRESS + VGA_WIDTH * (VGA_HEIGHT + 1) * 2)
#define NUMBER_BUFF_SIZE 0xF
#ifndef TERMINAL_H
#define TERMINAL_H



class TerminalStateBuffer;



enum TerminalColor {
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
	COLOR_WHITE = 15
};



typedef uint8_t ColorByte;



class Terminal {
public:

	class ColorScheme {
	public:
		ColorScheme(TerminalColor foreground, TerminalColor background){
			byte = makeColor(foreground, background);
		}
		~ColorScheme(){
		}
		ColorByte byte;
	private:
	};

	Terminal(TerminalColor foreground, TerminalColor background);
	Terminal();
	void redraw();
	void init();
	void clear();
	void setColor(TerminalColor foreground, TerminalColor background);
	void setColor(ColorScheme scheme);
	void setColor(ColorByte byte);
	void newLine();
	void putColoredCharAtPos(char c, ColorByte _color, size_t x, size_t y);
	void putchar(char c);
	void backspace();
	void puts(const char* string);
	void operator() (const char* string);
	void putsln(const char* string);
	void putint(int64_t value);
	void operator() (int64_t value);
	Terminal& operator<<(const char* string);
	Terminal& operator<<(char ch);
	Terminal& operator<<(int64_t value);
	Terminal& operator<<(uint64_t value);
	void putuint(uint64_t value);
	void puthex(uint64_t value);
	void putbytes(uint64_t value);
	static ColorByte makeColor(TerminalColor foreground, TerminalColor background);
	size_t getPosition();
	ColorByte getColor();
	void restoreFromBuffer(TerminalStateBuffer* buffer, bool empty = true);
	void saveToBuffer(TerminalStateBuffer* buffer);
	void setStatus(const char* status);
	void getStatus(char* status);
	int64_t getLocation();
	~Terminal();

private:
	char numberBuff[NUMBER_BUFF_SIZE];
	size_t row;
	size_t column;
	ColorByte color;
	uint16_t* startPtr;
	void putCharDefault(char c);
	void updateCursor();
	void scroll();
	uint16_t makeColoredChar(char c);
	uint16_t makeColoredChar(char c, ColorByte _color);
	friend class TerminalStateBuffer;
};



#endif /*TERMINAL_H*/