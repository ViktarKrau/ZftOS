#include "zftdef.h"
#include "interrupts.h"
#include "iob.h"
#include "enterqueue.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define TIMER_HARDWIRED_FREQUENCY 1193180
#define FREQUENCY 1


struct IDTDescr IDT[IDT_SIZE];

extern "C"	void timer_handler();
extern "C"  void keyboard_handler();
extern "C"  void load_idt(uint32_t ptrs[2]);
//Use C linkage for keyboard_handler.

extern "C"
void keyboard_handler_c(){
	uint8_t status;
	uint8_t keycode;

	status = inb(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = inb(KEYBOARD_DATA_PORT);
		enter_queue_push(keycode);
	}
		/* write EOI */
	outb(0x20, 0x20);
}

extern "C"
void timer_handler_c() {
	timerTick();
	outb(0x20, 0x20);
}
extern "C"
void initialize_timer() {
	uint32_t divisor = TIMER_HARDWIRED_FREQUENCY / FREQUENCY;
	outb(0x43, 0x34/*6*/);

	uint8_t l = (uint8_t) (divisor & 0xFF);

	// Send the frequency divisor.
	outb(0x40, l);
	uint8_t h = (uint8_t) ((divisor >> 8) & 0xFF);
	for(uint32_t delay = 0; delay < 0xFFFFFFFE; ++delay) {
	}													//Delay to init timer in proper way
	outb(0x40, h);
}

extern "C"
void initialize_idt(){
	uint32_t timer_handler_addrees;
	uint32_t keyboard_handler_address;
	uint32_t idt_address;
	uint32_t idt_ptr[2];

	/*KEYBOARD*/
	keyboard_handler_address = (uint32_t)keyboard_handler;
	IDT[0x21].offset_lowerbits = (uint16_t) (keyboard_handler_address & 0xffff);
	IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[0x21].offset_higherbits = (uint16_t) ((keyboard_handler_address & 0xffff0000) >> 16);

	/*TIMER*/
	timer_handler_addrees = (uint32_t)timer_handler;
	IDT[0x20].offset_lowerbits = (uint16_t) (timer_handler_addrees & 0xffff);
	IDT[0x20].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[0x20].zero = 0;
	IDT[0x20].type_attr = 0x8E; /* INTERRUPT_GATE */
	IDT[0x20].offset_higherbits = (uint16_t) ((timer_handler_addrees & 0xffff0000) >> 16);



	idt_address = (uint32_t)&IDT;
	idt_ptr[0] = (uint32_t) ((sizeof (struct IDTDescr) * IDT_SIZE) + ((idt_address & 0xffff) << 16));
	idt_ptr[1] = idt_address >> 16;
	load_idt(idt_ptr);

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/
	outb(0x20 , 0x11);
	outb(0xA0 , 0x11);

	/* ICW2 */
	outb(0x21 , 0x20);
	outb(0xA1 , 0x28);

	/* ICW3*/
	outb(0x21 , 0x04);
	outb(0xA1 , 0x02);

	/* ICW4*/
	outb(0x21 , 0x01);
	outb(0xA1 , 0x01);
	/* Initialization finished */

	/*Masks*/
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);


}
extern "C"
void unmask_interrupts(){
	outb(0x21 , 0xFC); // FD
}