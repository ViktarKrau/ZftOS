#include "displayclock.h"
#include "../kernel.h"
#include "../time.h"
#include "../string.h"
#define BUFFER_SIZE 16



DisplayClock::DisplayClock() : Executable(false) {

}


int DisplayClock::run(Vector<char*> args) {
    printArgs(args);
    while (true) {
        Time time = Time::getCurrentTime();

        putTimeEntity(time.day, '.', 60);
        putTimeEntity(time.month, '.', 63);
        putTimeEntity(time.year, ' ', 66);
        putTimeEntity(time.hour, ':', 69);
        putTimeEntity(time.minute, ':', 72);
        putTimeEntity(time.second, ' ', 75);
        //Kernel::scheduler.passControl();
    }
}



void DisplayClock::putTimeChar(char timeChar, size_t x) {
    Kernel::out.putColoredCharAtPos(timeChar
            , Terminal::makeColor(TerminalColor::COLOR_BLUE, TerminalColor::COLOR_LIGHT_GREY)
            , x, 0);
}



void DisplayClock::putTimeEntity(uint8_t entity, char splitSymbol, uint32_t startPosition) {
    char buffer[BUFFER_SIZE];
    int_to_string(entity, buffer, 10);
    putTimeChar(buffer[0], startPosition);
    putTimeChar(buffer[1], startPosition + 1);
    putTimeChar(splitSymbol, startPosition + 2);
}
