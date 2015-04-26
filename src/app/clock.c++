#include "clock.h"
#include "../kernel.h"


int Clock::run(Vector<char*> args) {
    Kernel::out << "IN CLOCK RUN()"<< "\n";
    Kernel::out.setStatus("\t\t\t\t\t CLOCK");
    printArgs(args);
    mainMenu();
    return 0;
}



Clock::Clock() : Executable(true) {

}



void Clock::mainMenu() {
    while (true) {
        Kernel::out.clear();
        Kernel::out.puts("Main menu: \n"
                                  "\t\t1. View time\n"
                                  "\t\t2. Set time\n"
                                  "\t\t3. Set alarm\n"
                                  "\t\t4. Delay(ms)\n"
                                  "\t\t5. Quit\n");
        switch (Kernel::in.getchar()) {
            case '1':
                printTime();
                break;
            case '2':
                setTime();
                break;
            case '3':
                setAlarm();
                break;
            case '4':
                runDelay();
                break;
            case '5':
                return;
            default:
                Kernel::out.puts("Wrong key\n");
                break;
        }
        Kernel::in.getchar();
    }
}



void Clock::printTime() {
    Time time = Time::getCurrentTime();
    Kernel::out.puts("Time: ");

    Kernel::out.putuint(time.day);
    Kernel::out.putchar('.');

    Kernel::out.putuint(time.month);
    Kernel::out.putchar('.');

    Kernel::out.putuint(time.year + 2000);
    Kernel::out.putchar(' ');

    Kernel::out.putuint(time.hour);
    Kernel::out.putchar(':');

    Kernel::out.putuint(time.minute);
    Kernel::out.putchar(':');

    Kernel::out.putuint(time.second);
}



void Clock::runDelay() {
    uint64_t delayTime;
    Kernel::out.puts("\nEnter time to delay in milliseconds: ");
    delayTime = Kernel::in.getuint();
    Kernel::out.puts("\nDelaying ");
    Kernel::out.putuint(delayTime);
    Kernel::out.puts(" milliseconds...");
    Time::delay(delayTime);
    Kernel::out.puts("\nDelay ended.");
    Kernel::in.flush();
}



void Clock::setTime() {
    Time time;


    safeRead(time.year, "\nEnter two last digits of current year: ", "\nWrong year, reenter pls", 99);
    safeRead(time.month, "\nEnter current month: ", "\nWrong month, reenter pls", 12);
    safeRead(time.day, "\nEnter current day of month: ", "\nWrong day, reenter pls", 31);
    safeRead(time.hour, "\nEnter current hour: ", "\nWrong hour, reenter plx", 24);
    safeRead(time.minute, "\nEnter current minute: ", "\nWrong minute, reenter plx", 60);
    safeRead(time.second, "\nEnter current second: ", "\nWrong second, reenter plx", 60);

    Time::setTime(time);
    Kernel::out.puts("\nTime is set to: \n");
    printTime();
}



void Clock::safeRead(uint8_t& param, const char* enterInvite, const char* errorMessage, uint8_t upperBound) {
    while (true) {
        Kernel::out.puts(enterInvite);
        param = (uint8_t) Kernel::in.getuint();
        if (param > upperBound) {
            Kernel::out.puts(errorMessage);
        }
        else {
            break;
        }
    }
}



void Clock::setAlarm() {
    uint8_t hour;
    uint8_t minute;
    safeRead(hour, "\nEnter alarm hour: ", "\nWrong hour, reenter plx", 24);
    safeRead(minute, "\nEnter alarm minute: ", "\nWrong minute, reenter plx", 60);
    Time::setAlarm(hour, minute);
    Kernel::out.puts("\nAlarm is set to ");
    Kernel::out.putuint(hour);
    Kernel::out.putchar(':');
    Kernel::out.putuint(minute);
}
