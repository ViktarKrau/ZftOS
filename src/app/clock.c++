#include "clock.h"
#include "../kernel.h"


int Clock::run(Vector<char *> args) {
    Kernel::out->setStatus("\t\t\t\t\t CLOCK");
    printArgs(args);
    mainMenu();
    return 0;
}



Clock::Clock() : Executable(true) {

}



void Clock::mainMenu() {
    while (true) {
        Kernel::out->clear();
        Kernel::out->puts("Main menu: \n"
                                  "\t\t1. View time\n"
                                  "\t\t2. Set time\n"
                                  "\t\t3. Set alarm\n"
                                  "\t\t4. Delay(ms)\n"
                                  "\t\t5. Quit\n");
        switch (Kernel::in->getchar()) {
            case '1':
                printTime();
                break;
            case '2':
                setTime();
                break;
            case '3':
                break;
            case '4':
                runDelay();
                break;
            case '5':
                return;
            default:
                Kernel::out->puts("Wrong key\n");
                break;
        }
        Kernel::in->getchar();
    }
}



void Clock::printTime() {
    Time time = Time::getCurrentTime();
    Kernel::out->puts("Time: ");

    Kernel::out->putuint(time.day);
    Kernel::out->putchar('.');

    Kernel::out->putuint(time.month);
    Kernel::out->putchar('.');

    Kernel::out->putuint(time.year + 2000);
    Kernel::out->putchar(' ');

    Kernel::out->putuint(time.hour);
    Kernel::out->putchar(':');

    Kernel::out->putuint(time.minute);
    Kernel::out->putchar(':');

    Kernel::out->putuint(time.second);
}



void Clock::runDelay() {
    uint64_t delayTime;
    Kernel::out->puts("\nEnter time to delay in milliseconds: ");
    delayTime = Kernel::in->getuint();
    Kernel::out->puts("\nDelaying ");
    Kernel::out->putuint(delayTime);
    Kernel::out->puts(" milliseconds...");
    Time::delay(delayTime);
    Kernel::out->puts("\nDelay ended.");
    Kernel::in->flush();
}



void Clock::setTime() {
    Time time;

    Kernel::out->puts("\nEnter two last digits of current year: ");
    time.year = (uint8_t) Kernel::in->getuint();

    Kernel::out->puts("\nEnter current month: ");
    time.month = (uint8_t) Kernel::in->getuint();

    Kernel::out->puts("\nEnter current day of month: ");
    time.day = (uint8_t) Kernel::in->getuint();

    Kernel::out->puts("\nEnter current hour: ");
    time.hour = (uint8_t) Kernel::in->getuint();

    Kernel::out->puts("\nEnter current minute: ");
    time.minute = (uint8_t) Kernel::in->getuint();

    Kernel::out->puts("\nEnter current second: ");
    time.second = (uint8_t) Kernel::in->getuint();

    Kernel::out->puts("Time is set to: \n");
    printTime();
}
