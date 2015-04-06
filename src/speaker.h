#include "zftdef.h"
#include "iob.h"
#include "time.h"

#ifndef _ZFTOS_DEV_SPEAKER_H_
#define _ZFTOS_DEV_SPEAKER_H_

#define TIMER_FREQUENCY 1193180U
#define BYTE_MAX_VALUE	256

class Speaker {
        public:
    static void play(uint32_t frequency, uint32_t duration);

    static void playDefaultMelody();

private:
    static void turnOff();

    static void turnOn();

    static void setFrequency(uint32_t frequency);
};



#endif //_ZFTOS_DEV_SPEAKER_H_
