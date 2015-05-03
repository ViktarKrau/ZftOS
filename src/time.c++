#include "zftdef.h"
#include "string.h"
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
#define RTC_SECONDS_REGISTER 0x00
#define RTC_SECONDS_ALARM_REGISTER 0x01
#define RTC_MINUTES_REGISTER 0x02
#define RTC_MINUTES_ALARM_REGISTER 0X03
#define RTC_HOURS_REGISTER 0x04
#define RTC_HOURS_ALARM_REGISTER 0X05
#define RTC_DAYS_REGISTER 0x07
#define RTC_MONTHS_REGISTER 0x08
#define RTC_YEARS_REGISTER 0x09
/*5th bit set*/
#define RTC_ENABLE_ALARM (0x01 << 5)
#define RTC_ENABLE_PERIODIC_INTERRUPT (0x01 << 6)
#define ALARM_TIMEOUT 5000
#define CMOS_PERIODIC_NORMAL_MODE (0x01 << 5)
/*1024 frequence, equal  976.5625 ms*/
#define CMOS_PERIODIC_SET_FREQUENCE (0x6)


char* Time::alarmMessageBuffer;
bool Time::needConvert;
bool Time::doesAlarmBlock = true;
bool Time::isClockFormatWrong;
int8_t Time::GMT;
volatile uint64_t Time::milliseconds;
volatile double Time::cmosMilliseconds;


void Time::initialize() {
    milliseconds = 0;
    GMT = 0;
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
    if(needConvert) {
        entity = getValueFromBCD(entity);
    }
    return entity;
}



uint8_t Time::getSecond() {
    return getConvertedTimeEntity(RTC_SECONDS_REGISTER);
}



uint8_t Time::getMinute() {
    return getConvertedTimeEntity(RTC_MINUTES_REGISTER);
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
    uint8_t hour = getTimeEntity(RTC_HOURS_REGISTER);
    hour = convertReadHour(hour);
    return hour;
}



uint8_t Time::getDay() {
    return getConvertedTimeEntity(RTC_DAYS_REGISTER);
}



uint8_t Time::getMonth() {
    return getConvertedTimeEntity(RTC_MONTHS_REGISTER);
}



uint8_t Time::getYear() {
    return getConvertedTimeEntity(RTC_YEARS_REGISTER);
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



void Time::setTime(const Time& time) {
    forbidUpdate();

    setYear(time.year);
    setMonth(time.month);
    setDay(time.day);
    setHour(time.hour);
    setMinute(time.minute);
    setSecond(time.second);

    allowUpdate();
}



void Time::setSecond(uint8_t second) {
    writeConvertedTimeEntity(RTC_SECONDS_REGISTER, second);
}



void Time::setMinute(uint8_t minute) {
    writeConvertedTimeEntity(RTC_MINUTES_REGISTER, minute);
}



void Time::setHour(uint8_t hour) {
    hour = convertHourToWrite(hour);
    writeTimeEntity(RTC_HOURS_REGISTER, hour);
}



void Time::setDay(uint8_t day) {
    writeConvertedTimeEntity(RTC_DAYS_REGISTER, day);
}



void Time::setMonth(uint8_t month) {
    writeConvertedTimeEntity(RTC_MONTHS_REGISTER, month);
}



void Time::setYear(uint8_t year) {
    writeConvertedTimeEntity(RTC_YEARS_REGISTER, year);
}



void Time::setAlarm(uint8_t hour, uint8_t minute, uint8_t second
        , const char* message, bool block) {
    writeConvertedTimeEntity(RTC_SECONDS_ALARM_REGISTER, second);
    writeConvertedTimeEntity(RTC_MINUTES_ALARM_REGISTER, minute);
    hour = convertHourToWrite(hour);
    writeTimeEntity(RTC_HOURS_ALARM_REGISTER, hour);
    alarmMessageBuffer = new char[strlen(message) + 1];
    size_t i;
    for (i = 0; message[i]; ++i) {
        alarmMessageBuffer[i] = message[i];
    }
    alarmMessageBuffer[i] = '\0';
    doesAlarmBlock = block;
    turnOnAlarm();
}



void Time::forbidUpdate() {
    while(isUpdateInProgress());
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    uint8_t rtcBRegister = inb(CMOS_DATA_PORT);
    rtcBRegister |= 128U;
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    outb(CMOS_DATA_PORT, rtcBRegister);
}



void Time::allowUpdate() {
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    uint8_t rtcBRegister = inb(CMOS_DATA_PORT);
    rtcBRegister ^= 128U;
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    outb(CMOS_DATA_PORT, rtcBRegister);
}



void Time::writeTimeEntity(uint8_t reg, uint8_t value) {
    outb(CMOS_STATUS_PORT, reg);
    outb(CMOS_DATA_PORT, value);
}



void Time::writeConvertedTimeEntity(uint8_t reg, uint8_t value) {
    if (needConvert) {
        value = convertValueToBCD(value);
    }
    writeTimeEntity(reg, value);
}



uint8_t Time::convertValueToBCD(uint8_t value) {
    return (uint8_t) ((value % 10) | ((value / 10) << 4));
}



void timeAlarm() {
    if (Time::alarmMessageBuffer == nullptr) {
        return;
    }
    if (Time::doesAlarmBlock) {
        Kernel::out.alarm(Time::alarmMessageBuffer, ALARM_TIMEOUT);
    }
    else {
        Kernel::out << "\nALARM:\n" << Time::alarmMessageBuffer << "\n";
    }
    delete Time::alarmMessageBuffer;
    Time::alarmMessageBuffer = nullptr;
}



uint8_t Time::convertHourToWrite(uint8_t hour) {
    if (needConvert) {
        hour = convertValueToBCD(hour);
    }
    if (!isClockFormatWrong) {
        if (hour > 12) {
            hour = (uint8_t) ((hour - 12) | 80);
        }
    }
    return hour;
}



uint8_t Time::convertReadHour(uint8_t hour) {
    if (needConvert) {
        //hour = getValueFromBCD(hour);
        hour = (uint8_t) (( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80));
    }
    if (isClockFormatWrong && (hour & 0x80)) {
        hour = (uint8_t) (((hour & 0x7F) + 12) % 24);
    }
    return hour;
}



void Time::turnOnAlarm() {
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    uint8_t bReg = inb(CMOS_DATA_PORT);
    bReg |= RTC_ENABLE_ALARM;
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    outb(CMOS_DATA_PORT, bReg);
}



void Time::turnOffAlarm() {
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    uint8_t bReg = inb(CMOS_DATA_PORT);
    bReg = (uint8_t) ((bReg | RTC_ENABLE_ALARM) ^ RTC_ENABLE_ALARM);
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    outb(CMOS_DATA_PORT, bReg);
}



uint8_t Time::getAlarmSecond() {
    return getConvertedTimeEntity(RTC_SECONDS_ALARM_REGISTER);
}



uint8_t Time::getAlarmMinute() {
    return getConvertedTimeEntity(RTC_MINUTES_ALARM_REGISTER);
}



uint8_t Time::getAlarmHour() {
    return getConvertedTimeEntity(RTC_HOURS_ALARM_REGISTER);
}



bool Time::isAlarmSet() {
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    return (bool) (inb(CMOS_DATA_PORT) & RTC_ENABLE_ALARM);
}



void cmosTimerTick() {
    Time::cmosMilliseconds += 0.9765625;
}



void Time::cmosDelay(uint64_t delayInMilliseconds) {
    turnOnRTCPeriodicInterrupt();
    cmosMilliseconds = 0.0;
    while (delayInMilliseconds > cmosMilliseconds) {
        halt();
    }
    turnOffRTCPeriodicInterrupt();

}



void Time::turnOnRTCPeriodicInterrupt() {
    outb(CMOS_STATUS_PORT, RTC_A_REGISTER);
    uint8_t aReg = inb(CMOS_DATA_PORT);
    aReg = (uint8_t) ((aReg
                       & UPDATE_IN_PROGRESS_FLAG)
               | CMOS_PERIODIC_NORMAL_MODE
               | CMOS_PERIODIC_SET_FREQUENCE);
    outb(CMOS_STATUS_PORT, RTC_A_REGISTER);
    outb(CMOS_DATA_PORT, aReg);

    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    uint8_t bReg = inb(CMOS_DATA_PORT);
    bReg |= RTC_ENABLE_PERIODIC_INTERRUPT;
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    outb(CMOS_DATA_PORT, bReg);
}



void Time::turnOffRTCPeriodicInterrupt() {
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    uint8_t bReg = inb(CMOS_DATA_PORT);
    bReg = (uint8_t) ((bReg | RTC_ENABLE_PERIODIC_INTERRUPT)
                      ^ RTC_ENABLE_PERIODIC_INTERRUPT);
    outb(CMOS_STATUS_PORT, RTC_B_REGISTER);
    outb(CMOS_DATA_PORT, bReg);
}
