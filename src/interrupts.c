#include "zftdef.h"
#include "interrupts.h"
#include "iob.h"
#include "enterqueue.h++"
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64



struct IDTDescr IDT[IDT_SIZE];

extern "C"  void keyboard_handler();
extern "C"  void load_idt(uint32_t ptrs[2]);
//Use C linkage for keyboard_handler.
#if defined(__cplusplus)
extern "C" 
#endif
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
void init_idt(){

	uint32_t keyboard_address;
	uint32_t idt_address;
	uint32_t idt_ptr[2];
	
	keyboard_address = (uint32_t)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
	

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	outb(0x20 , 0x11);
	outb(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	outb(0x21 , 0x20);
	outb(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	outb(0x21 , 0x00);  
	outb(0xA1 , 0x00);  

	/* ICW4 - environment info */
	outb(0x21 , 0x00);
	outb(0xA1 , 0x00);
	/* Initialization finished */

	/* mask interrupts */
	outb(0x21 , 0xff);
	outb(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (uint32_t)IDT ;
	idt_ptr[0] = (sizeof (struct IDTDescr) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;
	load_idt(idt_ptr);
}
extern "C"
void enable_kb(){
	outb(0x21 , 0xFD);
}