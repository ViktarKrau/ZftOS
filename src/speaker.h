#include "zftdef.h"
#include "iob.h"
#include "time.h"

#ifndef _ZFTOS_DEV_SPEAKER_H_
#define _ZFTOS_DEV_SPEAKER_H_

#define TIMER_FREQUENCY 1193180U
#define BYTE_MAX_VALUE	256

class Speaker {
public:
    class Sound {
    public:
        Sound();
        Sound(uint32_t _frequency, uint32_t _duration);
        uint32_t frequency;
        uint32_t duration;
    };
    static void play(uint32_t frequency, uint32_t duration);
    static void play(Sound);
    static void playDefaultMelody();
    static bool playWithKeyboardBlock(uint32_t frequency, uint64_t millisecTimeout);
    static void playWithKeyboradBlock(uint32_t frequency);
private:
    static void turnOff();
    static void turnOn();
    static void setFrequency(uint32_t frequency);
};



#endif //_ZFTOS_DEV_SPEAKER_H_
