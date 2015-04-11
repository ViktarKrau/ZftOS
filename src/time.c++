#include <stdint-gcc.h>
#include "time.h"
#include "iob.h"
#include "kernel.h"



#define CMOS_STATUS_PORT 0x70
#define CMOS_DATA_PORT 0x71
#define RTC_B_REGISTER 0x0B
#define RTC_A_REGISTER 0x0A
#define HOUR_FORMAT_FLAG 0x02
#define BCD_CLOCK_FORMAT_FLAG 0x04
#define UPDATE_IN_PROGRESS_FLAG 0x80
#define READ_SECONDS_COMMAND 0x00
#define READ_MINUTES_COMMAND 0x02
#define READ_HOURS_COMMAND 0x04
#define READ_DAYS_COMMAND 0x07
#define READ_MONTHS_COMMAND 0x08
#define READ_YEARS_COMMAND 0x09



bool Time::needConvert;
bool Time::isClockFormatWrong;
int8_t Time::GMT;
volatile uint64_t Time::milliseconds;



void Time::initialize() {
    milliseconds = 0;
    GMT = 3;
    needConvert = !((bool) (getRTCRegister(RTC_B_REGISTER) & BCD_CLOCK_FORMAT_FLAG));
    isClockFormatWrong = (bool) (getRTCRegister(RTC_B_REGISTER) & HOUR_FORMAT_FLAG);
}



bool Time::isUpdateInProgress() {
    outb(CMOS_STATUS_PORT, RTC_A_REGISTER);
    return (bool) (inb(CMOS_DATA_PORT) & UPDATE_IN_PROGRESS_FLAG);
}



uint8_t Time::getRTCRegister(uint8_t reg) {
    outb(CMOS_STATUS_PORT, reg);
    return inb(CMOS_DATA_PORT);
}



uint8_t Time::getTimeEntity(uint8_t reg)  {
    while(isUpdateInProgress());
    uint8_t retval = getRTCRegister(reg);
    uint8_t lastRetval;
    do {
        lastRetval = retval;
        while (isUpdateInProgress());
        retval = getRTCRegister(reg);
    } while(lastRetval != retval);
    return retval;
}



uint8_t Time::getValueFromBCD(uint8_t BCD) {
    return (uint8_t) ((BCD & 0x0F) + ((BCD / 16) * 10));
}



uint8_t Time::getConvertedTimeEntity(uint8_t reg) {
    uint8_t entity = getTimeEntity(reg);
    if(needConvert){
        entity = getValueFromBCD(entity);
    }
    return entity;
}



uint8_t Time::getSecond() {
    return getConvertedTimeEntity(READ_SECONDS_COMMAND);
}



uint8_t Time::getMinute() {
    return getConvertedTimeEntity(READ_MINUTES_COMMAND);
}



void Time::update(Time& time) {
    time.second = getSecond();
    time.minute = getMinute();
    time.hour = getHour();
    time.day = getDay();
    time.month = getMonth();
    time.year = getYear();
}






uint8_t Time::getHour() {
    uint8_t hour = getTimeEntity(READ_HOURS_COMMAND);
    if (needConvert) {
        //hour = getValueFromBCD(hour);
        hour = (uint8_t) (( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80));
    }
    if (isClockFormatWrong && (hour & 0x80)) {
        hour = (uint8_t) (((hour & 0x7F) + 12) % 24);
    }
    return hour;
}



uint8_t Time::getDay() {
    return getConvertedTimeEntity(READ_DAYS_COMMAND);
}



uint8_t Time::getMonth() {
    return getConvertedTimeEntity(READ_MONTHS_COMMAND);
}



uint8_t Time::getYear() {
    return getConvertedTimeEntity(READ_YEARS_COMMAND);
}



Time Time::getCurrentTime() {
    static const uint8_t daysInMonth[] = {31, 28, 31, 30/*APRIL*/, 31, 30, 31, 31, 30, 31, 30, 31};
    Time retval;
    retval.year = getYear();
    retval.month = getMonth();
    retval.day = getDay();
    retval.hour = getHour() + GMT;
    if (retval.hour >= 24 && retval.hour < 100) {
        retval.hour -= 24;
        retval.day += 1;
        if (retval.day > daysInMonth[retval.month + 1]) {
            retval.month++;
            retval.day = 1;
            if (retval.month > 12){
                retval.month = 1;
                retval.year++;
            }
        }
    }
    else if (retval.hour > 100) /*As far as hour is uint8_t, it cant be less than zero, it will just
*                               overflow*/ {
        retval.hour += 24;
        retval.day -= 1;
        if (retval.day <= 0) {
            retval.month--;
            if (retval.month == 0){
                retval.month = 12;
                retval.year--;
            }
            retval.day = daysInMonth[retval.month - 1];

        }
    }
    retval.minute = getMinute();
    retval.second = getSecond();

    return retval;
}



Time::Time(uint8_t _second, uint8_t _minute, uint8_t _hour, uint8_t _day, uint8_t _month, uint8_t _year):
            second(_second), minute(_minute), hour(_hour), day(_day), month(_month), year(_year){
}



void Time::setGMT(int8_t value) {
    if (value > 12 || value < -12) {
        return;
    }
    GMT = value;
}



Time::Time() {
}



void Time::delay(uint64_t delayInMilliseconds) {
    uint64_t end = delayInMilliseconds + milliseconds;
    while (getMilliseconds() < end) {
        halt();
    }
}



uint64_t Time::getMilliseconds() {
    return milliseconds;
}



void timerTick() {
    Time::milliseconds += 10;
}



int8_t Time::getGMT() {
    return GMT;
}

/*TODO: IMPLEMENT THESE*/

void Time::setTime(const Time& time) {
    setYear(time.year);
    setMonth(time.month);
    setDay(time.day);
    setHour(time.hour);
    setMinute(time.minute);
    setSecond(time.second);
}



void Time::setSecond(uint8_t second) {

}



void Time::setMinute(uint8_t minute) {

}



void Time::setHour(uint8_t hour) {

}



void Time::setDay(uint8_t day) {

}



void Time::setMonth(uint8_t month) {

}



void Time::setYear(uint8_t year) {

}
