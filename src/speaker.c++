#include <stdint-gcc.h>
#include <stdint.h>
#include "speaker.h"
#include "kernel.h"

#define SPEAKER_CONTROL_PORT 0x61
#define TURN_OFF 0xFD
#define DISABLE_SOUND 0xFE
#define TURN_ON 0x02
#define ENABLE_SOUND 0x01
#define TIMER_CONTROL_PORT 0x43
#define TIMER_2ND_CHANNEL_PORT 0x42




void Speaker::turnOff() {
    outb(SPEAKER_CONTROL_PORT, (uint8_t) (inb(SPEAKER_CONTROL_PORT) & TURN_OFF & DISABLE_SOUND));
}



void Speaker::playDefaultMelody() {
    play(349, 600);
    play(392, 300);
    play(440, 600);
    play(349, 300);
    play(440, 300);
    play(440, 300);
    play(392, 300);
    play(349, 300);
    play(392, 300);
}



void Speaker::play(uint32_t frequency, uint32_t duration) {
    if (frequency == 0 || duration == 0) {
        return;
    }
    setFrequency(frequency);
    turnOn();
    Time::delay(duration);
    turnOff();
}



void Speaker::turnOn() {
    outb(SPEAKER_CONTROL_PORT, (uint8_t) (inb(SPEAKER_CONTROL_PORT) | TURN_ON | ENABLE_SOUND));
}



void Speaker::setFrequency(uint32_t frequency) {
    uint32_t divider = TIMER_FREQUENCY / frequency;
    outb(TIMER_CONTROL_PORT, 0xB6);						                            //START INIT
    outb(TIMER_2ND_CHANNEL_PORT, (uint8_t) (divider % BYTE_MAX_VALUE));	            //LOW
    divider /= BYTE_MAX_VALUE;
    outb(TIMER_2ND_CHANNEL_PORT, (uint8_t) divider);				 	            //HIGH
}



bool Speaker::playWithKeyboardBlock(uint32_t frequency, uint64_t millisecTimeout) {
    if (frequency == 0) {
        return true;
    }
    setFrequency(frequency);
    uint64_t startTime = Time::getMilliseconds();
    turnOn();
    char c = 0;
    while (Time::getMilliseconds() < startTime + millisecTimeout
            && (c = Kernel::in->getkey_nolock()) == 0) {
        halt();
    }
    turnOff();
    return c != 0;
}



void Speaker::playWithKeyboradBlock(uint32_t frequency) {
    if (frequency == 0) {
        return;
    }
    setFrequency(frequency);
    turnOn();
    while (Kernel::in->getkey_nolock() == 0) {
        halt();
    }
    turnOff();
}



void Speaker::play(Speaker::Sound sound) {
    play(sound.frequency, sound.duration);
}



Speaker::Sound::Sound() {
    frequency = 0;
    duration = 0;
}



Speaker::Sound::Sound(uint32_t _frequency, uint32_t _duration)
        : frequency(_frequency)
        , duration(_duration) {
}
