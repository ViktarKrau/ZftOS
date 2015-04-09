#ifndef _ZFTOS_DEV_TERMINALSTATEBUFFER_H_
#define _ZFTOS_DEV_TERMINALSTATEBUFFER_H_

#include "terminal.h"

class TerminalStateBuffer {
public:
    TerminalStateBuffer();
    TerminalStateBuffer(Terminal* terminal);
    void saveFrom(Terminal* terminal);
    void loadTo(Terminal* terminal);
    ~TerminalStateBuffer();
private:
    uint16_t* screen;
    char numberBuff[NUMBER_BUFF_SIZE];
    size_t row;
    size_t column;
    ColorByte color;
};

#endif //_ZFTOS_DEV_TERMINALSTATEBUFFER_H_
