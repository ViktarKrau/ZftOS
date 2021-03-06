#include "terminal.h"


#ifndef _ZFTOS_DEV_TERMINALSTATEBUFFER_H_
#define _ZFTOS_DEV_TERMINALSTATEBUFFER_H_



class TerminalStateBuffer {
public:
    TerminalStateBuffer();
    TerminalStateBuffer(Terminal* terminal);
    TerminalStateBuffer(Terminal& terminal);
    void saveFrom(Terminal* terminal);
    void saveFrom(Terminal& terminal);
    void loadTo(Terminal* terminal);
    void empty();
    ~TerminalStateBuffer();
private:
    uint16_t* screen;
    char numberBuff[NUMBER_BUFF_SIZE];
    size_t row;
    size_t column;
    ColorByte color;
};



#endif //_ZFTOS_DEV_TERMINALSTATEBUFFER_H_
