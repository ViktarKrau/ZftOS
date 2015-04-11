#include "zftdef.h"



#ifndef IOB_H
#define IOB_H



static __inline void outb(uint16_t port, uint8_t value) {
	__asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}



static __inline uint8_t inb(uint16_t port) {
	uint8_t value;
	__asm__ __volatile__ ("inb %w1,%0":"=a" (value):"Nd" (port));
	return value;
}



static __inline void halt() {
	__asm__ ("hlt");
}



static __inline void enable_interrupts() {
	__asm__ ("sti");
}



static __inline void disable_interrupts() {
	__asm__ ("cli");
}



#endif