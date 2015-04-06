#include <stdint-gcc.h>
#include "time.h"
#include "iob.h"
#include "kernel.h"
#define CMOS_ADRESS 0x70
#define CMOS_DATA 0x71

bool Time::needConvert;
bool Time::isClockFormatWrong;
int8_t Time::GMT;
volatile uint64_t Time::milliseconds;

void Time::initialize(){
    milliseconds = 0;
    GMT = 3;
    needConvert = !((bool) (getRTCRegister(0x0B) & 0x04));
    isClockFormatWrong = (bool) (getRTCRegister(0x0B) & 0x02);
}



bool Time::isUpdateInProgress() {
    outb(CMOS_ADRESS, 0x0A);
    return (bool) (inb(CMOS_DATA) & 0x80);
}



uint8_t Time::getRTCRegister(uint8_t reg) {
    outb(CMOS_ADRESS, reg);
    return inb(CMOS_DATA);
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
    return (uint8_t) ((BCD & 0x0F) + ((BCD / 16) * 10));;
}



uint8_t Time::getConvertedTimeEntity(uint8_t reg) {
    uint8_t entity = getTimeEntity(reg);
    if(needConvert){
        entity = getValueFromBCD(entity);
    }
    return entity;
}



uint8_t Time::getSecond() {
    return getConvertedTimeEntity(0x00);
}



uint8_t Time::getMinute() {
    return getConvertedTimeEntity(0x02);
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
    uint8_t hour = getTimeEntity(0x04);
    if(needConvert){
        //hour = getValueFromBCD(hour);
        hour = (uint8_t) (( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80));
    }
    if(isClockFormatWrong && (hour & 0x80)){
        hour = (uint8_t) (((hour & 0x7F) + 12) % 24);
    }
    return hour;
}



uint8_t Time::getDay() {
    return getConvertedTimeEntity(0x07);
}



uint8_t Time::getMonth() {
    return getConvertedTimeEntity(0x08);
}



uint8_t Time::getYear() {
    return getConvertedTimeEntity(0x09);
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
