#include "speaker.h"



void Speaker::turnOff() {
    outb(0x61, (uint8_t) (inb(0x61) & 0xFC));
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
    outb(0x61, (uint8_t) (inb(0x61) | 0x03));
}



void Speaker::setFrequency(uint32_t frequency) {
    uint32_t divider = TIMER_FREQUENCY / frequency;
    outb(0x43, 0xB6);						            //START INIT
    outb(0x42, (uint8_t) (divider % BYTE_MAX_VALUE));	//LOW
    divider /= BYTE_MAX_VALUE;
    outb(0x42, (uint8_t) divider);				 	    //HIGH
}