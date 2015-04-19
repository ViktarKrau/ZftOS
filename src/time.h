#include "zftdef.h"

#ifndef _ZFTOS_DEV_TIME_H
#define _ZFTOS_DEV_TIME_H


class Time {
public:
    Time();
    Time(uint8_t _second, uint8_t _minute, uint8_t _hour, uint8_t _day, uint8_t _month, uint8_t _year);
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;

    static void delay(uint64_t delayInMilliseconds);
    static void initialize();
    static Time getCurrentTime();
    static void update(Time& time);
    static uint8_t getSecond();
    static uint8_t getMinute();
    static uint8_t getHour();
    static uint8_t getDay();
    static uint8_t getMonth();
    static uint8_t getYear();
    static int8_t getGMT();
    static void setTime(const Time& time);
    static void setAlarm(uint8_t hour, uint8_t minute);
    static void setGMT(int8_t value);
    friend void timerTick();
    static uint64_t getMilliseconds();
private:
    static void setSecond(uint8_t second);
    static void setMinute(uint8_t minute);
    static void setHour(uint8_t hour);
    static void setDay(uint8_t day);
    static void setMonth(uint8_t month);
    static void setYear(uint8_t year);
    static void forbidUpdate();
    static void allowUpdate();
    static volatile uint64_t milliseconds;
    static int8_t GMT;
    static bool needConvert;
    static bool isClockFormatWrong;
    static bool isUpdateInProgress();
    static uint8_t getRTCRegister(uint8_t reg);
    static uint8_t getValueFromBCD(uint8_t BCD);
    static uint8_t convertValueToBCD(uint8_t value);
    static uint8_t getTimeEntity(uint8_t reg);
    static void writeTimeEntity(uint8_t reg, uint8_t value);
    static uint8_t getConvertedTimeEntity(uint8_t reg);
    static void writeConvertedTimeEntity(uint8_t reg, uint8_t value);
};


#endif //_ZFTOS_DEV_TIME_H_
