#include "../exec/executable.h"



#ifndef ZFTOS_DEV_DISPLAYCLOCK_H
#define ZFTOS_DEV_DISPLAYCLOCK_H



class DisplayClock : Executable {
public:
    DisplayClock();
    virtual int run(Vector<char*> args) override;
private:
    void putTimeEntity(uint8_t entity, char divider, uint32_t startPosition);
    void putTimeChar(char timeChar, size_t x);
};



#endif //ZFTOS_DEV_DISPLAYCLOCK_H
