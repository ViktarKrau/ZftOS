#include <stdint-gcc.h>
#include "zftdef.h"
#ifndef _ZFTOS_DEV_TIME_H
#define _ZFTOS_DEV_TIME_H

class Time {
public:
    Time(uint8_t _second, uint8_t _minute, uint8_t _hour, uint8_t _day, uint8_t _month, uint8_t _year);
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;


    static void init();
    static Time getCurrentTime();
    static void update(Time& time);
    static uint8_t getSecond();
    static uint8_t getMinute();
    static uint8_t getHour();
    static uint8_t getDay();
    static uint8_t getMonth();
    static uint8_t getYear();
private:
    static bool needConvert;
    static bool isClockFormatWrong;
    static bool isUpdateInProgress();
    static uint8_t getRTCRegister(uint8_t reg);
    static uint8_t getValueFromBCD(uint8_t BCD);
    static uint8_t getTimeEntity(uint8_t reg);
    static uint8_t getConvertedTimeEntity(uint8_t reg);
};


#endif //_ZFTOS_DEV_TIME_H_
