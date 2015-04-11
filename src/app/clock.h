#include "../exec/executable.h"
#include "../time.h"


#ifndef ZFTOS_DEV_CLOCK_H
#define ZFTOS_DEV_CLOCK_H



class Clock : public Executable {
public:
    Clock();
    virtual int run(Vector<char*> args) override;
private:
    void mainMenu();
    void runDelay();
    void setTime();
    void printTime();
};



#endif //ZFTOS_DEV_CLOCK_H
