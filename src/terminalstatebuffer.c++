#include "terminalstatebuffer.h"
#include "zft_memory.h"



TerminalStateBuffer::TerminalStateBuffer() {
    screen = nullptr;
    *numberBuff = 0;
    row = 0;
    color = 0;
    column = 0;
}



TerminalStateBuffer::TerminalStateBuffer(Terminal *terminal) {
    saveFrom(terminal);
}



TerminalStateBuffer::~TerminalStateBuffer() {
    delete []screen;
}



void TerminalStateBuffer::saveFrom(Terminal *terminal) {
    if (screen) {
        return;
    }
    screen = new uint16_t[VGA_HEIGHT * VGA_WIDTH];
    memcpy((void*)screen, (void*)(0xB800), VGA_HEIGHT * VGA_WIDTH * 2);
    for (size_t i = 0; i < NUMBER_BUFF_SIZE; ++i) {
        numberBuff[i] = terminal->numberBuff[i];
    }
    column = terminal->column;
    row = terminal->row;
    color = terminal->color;
}



void TerminalStateBuffer::loadTo(Terminal *terminal) {
    memcpy((void*)(0xB800), (void*)screen, VGA_HEIGHT * VGA_WIDTH * 2);
    for (size_t i = 0; i < NUMBER_BUFF_SIZE; ++i) {
        terminal->numberBuff[i] = numberBuff[i];
    }
    terminal->column = column;
    terminal->row = row;
    terminal->color = color;
}
