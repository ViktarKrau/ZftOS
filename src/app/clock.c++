#include "clock.h"
#include "../kernel.h"


#define ALARM_MESSAGE_BUFFER_SIZE 64
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
                                  "\t\t5. View alarm time\n"
                                  "\t\t6. Turn off alarm\n"
                                  "\t\t7. Quit\n");
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
                Kernel::out << "\n" << Time::getAlarmHour()
                << ":" << Time::getAlarmMinute() << ":" << Time::getAlarmSecond();
                Kernel::out << ((Time::isAlarmSet())? "\nalarm is set" : "alarm is not set");
                break;
            case '6':
                Kernel::out << "Alarm is not set now";
                Time::turnOffAlarm();
                break;
            case '7':
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


    safeRead(time.year, "\nEnter two last digits of current year: ", "\nWrong year, reenter pls", 99, true);
    safeRead(time.month, "\nEnter current month: ", "\nWrong month, reenter pls", 12, false);
    safeRead(time.day, "\nEnter current day of month: ", "\nWrong day, reenter pls", 31, false);
    safeRead(time.hour, "\nEnter current hour: ", "\nWrong hour, reenter plx", 24, true);
    safeRead(time.minute, "\nEnter current minute: ", "\nWrong minute, reenter plx", 60, true);
    safeRead(time.second, "\nEnter current second: ", "\nWrong second, reenter plx", 60, true);

    Time::setTime(time);
    Kernel::out.puts("\nTime is set to: \n");
    printTime();
}



void Clock::safeRead(uint8_t& param, const char* enterInvite
        , const char* errorMessage, uint8_t upperBound, bool canBeZero) {
    while (true) {
        Kernel::out.puts(enterInvite);
        param = (uint8_t) Kernel::in.getuint();
        if (param > upperBound || (!canBeZero && param == 0)) {
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
    uint8_t second;
    safeRead(hour, "\nEnter alarm hour: ", "\nWrong hour, reenter plx", 24, true);
    safeRead(minute, "\nEnter alarm minute: ", "\nWrong minute, reenter plx", 60, true);
    safeRead(second, "\nEnter alarm second: ", "\nWrong second, reenter plx", 60, true);

    char buffer[ALARM_MESSAGE_BUFFER_SIZE];
    Kernel::out << "\nEnter alarm message: ";
    Kernel::in.gets(buffer, ALARM_MESSAGE_BUFFER_SIZE);

    Kernel::out << "\nBlock?(Y/N)";
    char c = Kernel::in.getchar();

    Time::setAlarm(hour, minute, second, buffer, (c == 'y'));
    Kernel::out.puts("\nAlarm is set to ");
    Kernel::out.putuint(Time::getAlarmHour());
    Kernel::out.putchar(':');
    Kernel::out.putuint(Time::getAlarmMinute());
    Kernel::out.puts(":");
    Kernel::out.putuint(Time::getAlarmSecond());
}
