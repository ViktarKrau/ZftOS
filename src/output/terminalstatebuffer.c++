#include "terminalstatebuffer.h"
#include "../zft_memory.h"



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
    if (screen != nullptr) {
        delete[]screen;
    }
}



void TerminalStateBuffer::saveFrom(Terminal *terminal) {
    screen = new uint16_t[VGA_HEIGHT * VGA_WIDTH];
    memcpy((void*)screen, (void*)(VGA_START_ADDRESS), VGA_HEIGHT * VGA_WIDTH * 2);
    for (size_t i = 0; i < NUMBER_BUFF_SIZE; ++i) {
        numberBuff[i] = terminal->numberBuff[i];
    }
    column = terminal->column;
    row = terminal->row;
    color = terminal->color;
}



void TerminalStateBuffer::loadTo(Terminal *terminal) {
    memcpy((void*)(VGA_START_ADDRESS), (void*)screen, VGA_HEIGHT * VGA_WIDTH * 2);
    for (size_t i = 0; i < NUMBER_BUFF_SIZE; ++i) {
        terminal->numberBuff[i] = numberBuff[i];
    }
    terminal->column = column;
    terminal->row = row;
    terminal->color = color;
}



void TerminalStateBuffer::empty() {
    if (screen != nullptr) {
        delete[] screen;
        screen = nullptr;
    }
    *numberBuff = 0;
    row = 0;
    color = 0;
    column = 0;
}



void TerminalStateBuffer::saveFrom(Terminal& terminal) {
    screen = new uint16_t[VGA_HEIGHT * VGA_WIDTH];
    memcpy((void*)screen, (void*)(VGA_START_ADDRESS), VGA_HEIGHT * VGA_WIDTH * 2);
    for (size_t i = 0; i < NUMBER_BUFF_SIZE; ++i) {
        numberBuff[i] = terminal.numberBuff[i];
    }
    column = terminal.column;
    row = terminal.row;
    color = terminal.color;
}



TerminalStateBuffer::TerminalStateBuffer(Terminal& terminal) {
    saveFrom(terminal);
}
