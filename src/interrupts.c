#include "zftdef.h"
#include "interrupts.h"
#include "iob.h"
#include "input/enterqueue.h"



#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define TIMER_HARDWIRED_FREQUENCY 1193180
#define FREQUENCY 1
#define PIC_MASTER_CONTROL_PORT 0x20
#define PIC_SLAVE_CONTROL_PORT 0xA0
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_DATA_PORT 0xA1
#define ICW1_INIT 0x10
#define ICW1_ENABLE_ICW4 0x01
#define ICW2_MASTER_OFFSET_INTERRUPT_SERVICE_ROUTINES 0x20
#define ICW2_SLAVE_OFFSET_INTERRUPT_SERVICE_ROUTINES 0x28
#define ICW3_MASTER_2ND_LINE_CONNECTED_TO_SLAVE 0x04
#define ICW3_SLAVE_CONNECTED_TO_MASTER_2ND_LINE 0x02
#define ICW4_86_MODE 0x01
#define EOI	0x20
#define WRITE_MASTER_EOI() outb(PIC_MASTER_CONTROL_PORT, EOI)
#define WRITE_SLAVE_EOI() outb(PIC_SLAVE_CONTROL_PORT, EOI)
#define TIMER_CONTROL_PORT 0x43
#define TIMER_ZERO_CHANNEL_SQUARE_WAVE_MODE_INITIALIZE 0x34
#define TIMER_ZERO_CHANNEL_PORT 0x40
#define KEYBOARD_INTERRUPT_ENTRY_NUMBER 0x21
#define TIMER_INTERRUPT_ENTRY_NUMBER 0x20
#define CMOS_INTERRUPT_ENTRY_NUMBER 0x28
#define PAGE_FAULT_INTERRUPT_ENTRY_NUMBER 0xE
#define INTERRUPT_GATE 0x8E
#define FULL_MASK 0xFF
#define UNMASK_TIMER_INTERRUPT 0xFE
#define UNMASK_KEYBOARD_INTERRUPT 0xFD
#define UNMASK_SLAVE_INTERRUPT 0xFB
#define UNMASK_RTC_INTERRUPT 0xFE
#define GET_LOWER_WORD(x) (x & 0xFFFF)
#define GET_HIGHER_WORD(x) ((x & 0xffff0000) >> 16)
#define IS_LOWEST_BIT_SET(x) (x & 0x1)
#define GET_LOWER_BYTE(x) (x & 0xFF)
#define GET_HIGHER_BYTE(x) ((x >> 8) & 0xFF)
#define WORD_BITS_COUNT 16
#define CMOS_STATUS_PORT 0x70
#define CMOS_DATA_PORT 0x71
#define RTC_C_REGISTER 0x0C
#define CMOS_ALARM_INTERRUPT_BIT (0x01 << 5)
#define CMOS_PERIODICAL_INTERRUPT_BIT (0x01 << 6)



struct IDTDescr IDT[IDT_SIZE];



extern "C"	void timer_handler();
extern "C"  void keyboard_handler();
extern "C"  void cmos_handler();
extern "C"  void load_idt(uint32_t ptrs[2]);
void switchTasks();



extern "C"
void keyboard_handler_c() {
	uint8_t status;
	uint8_t keycode;

	status = inb(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (IS_LOWEST_BIT_SET(status)) {
		keycode = inb(KEYBOARD_DATA_PORT);
		enter_queue_push(keycode);
	}
	WRITE_MASTER_EOI();
	WRITE_SLAVE_EOI();
}



extern "C"
void cmos_handler_c() {
	outb(CMOS_STATUS_PORT, RTC_C_REGISTER);
	uint8_t interrupt_type = inb(CMOS_DATA_PORT);
	if (interrupt_type & CMOS_ALARM_INTERRUPT_BIT) {
		timeAlarm();
	}
	else if (interrupt_type & CMOS_PERIODICAL_INTERRUPT_BIT) {
		cmosTimerTick();
	}
	WRITE_SLAVE_EOI();
	WRITE_MASTER_EOI();
}



extern "C"
void page_fault_handler() {
	*(char*)(0xB8000) = 'A';
	WRITE_MASTER_EOI();
}



extern "C"
void timer_handler_c() {
	timerTick();
	//switchTasks();
	WRITE_MASTER_EOI();
}



extern "C"
void initialize_timer() {
	uint32_t divisor = TIMER_HARDWIRED_FREQUENCY / FREQUENCY;
	outb(TIMER_CONTROL_PORT, TIMER_ZERO_CHANNEL_SQUARE_WAVE_MODE_INITIALIZE);

	uint8_t lower_bits = (uint8_t) (GET_LOWER_BYTE(divisor));

	// Send the frequency divisor.
	outb(TIMER_ZERO_CHANNEL_PORT, lower_bits);
	uint8_t higher_bits = (uint8_t) (GET_HIGHER_BYTE(divisor));
	//MIGHT NEED DELAY HERE!!!!
	outb(TIMER_ZERO_CHANNEL_PORT, higher_bits);
}



extern "C"
void make_idt_entry (struct IDTDescr* descr, void (*handler)(), uint16_t selector, uint8_t type_attr) {
	uint32_t handler_address = (uint32_t)handler;
	descr->offset_lowerbits = (uint16_t) (GET_LOWER_WORD(handler_address));
	descr->selector = selector;
	descr->zero = 0;
	descr->type_attr = type_attr;
	descr->offset_higherbits = (uint16_t) (GET_HIGHER_WORD(handler_address));
}



extern "C"
void initialize_idt() {
	get_cs();

	uint32_t idt_address;
	uint32_t idt_ptr[2];


	make_idt_entry(IDT + KEYBOARD_INTERRUPT_ENTRY_NUMBER, keyboard_handler, (uint16_t)__cs, INTERRUPT_GATE);
	make_idt_entry(IDT + TIMER_INTERRUPT_ENTRY_NUMBER, timer_handler, (uint16_t)__cs, INTERRUPT_GATE);
	make_idt_entry(IDT + PAGE_FAULT_INTERRUPT_ENTRY_NUMBER, page_fault_handler, (uint16_t)__cs, INTERRUPT_GATE);
	make_idt_entry(IDT + CMOS_INTERRUPT_ENTRY_NUMBER, cmos_handler, (uint16_t)__cs, INTERRUPT_GATE);

	idt_address = (uint32_t)&IDT;
	idt_ptr[0] = (uint32_t) ((sizeof (struct IDTDescr) * IDT_SIZE)
			+ (GET_LOWER_WORD(idt_address) << WORD_BITS_COUNT));
	idt_ptr[1] = GET_HIGHER_WORD(idt_address);
	load_idt(idt_ptr);

	/*ICW1*/
	outb(PIC_MASTER_CONTROL_PORT, ICW1_ENABLE_ICW4 | ICW1_INIT);
	outb(PIC_SLAVE_CONTROL_PORT, ICW1_ENABLE_ICW4 | ICW1_INIT);

	/* ICW2 */
	outb(PIC_MASTER_DATA_PORT, ICW2_MASTER_OFFSET_INTERRUPT_SERVICE_ROUTINES);
	outb(PIC_SLAVE_DATA_PORT, ICW2_SLAVE_OFFSET_INTERRUPT_SERVICE_ROUTINES);

	/* ICW3*/
	outb(PIC_MASTER_DATA_PORT, ICW3_MASTER_2ND_LINE_CONNECTED_TO_SLAVE);
	outb(PIC_SLAVE_DATA_PORT, ICW3_SLAVE_CONNECTED_TO_MASTER_2ND_LINE);

	/* ICW4*/
	outb(PIC_MASTER_DATA_PORT, ICW4_86_MODE);
	outb(PIC_SLAVE_DATA_PORT, ICW4_86_MODE);
	/* Initialization finished */

	/*Masks*/
	outb(PIC_MASTER_DATA_PORT, FULL_MASK);
	outb(PIC_SLAVE_DATA_PORT, FULL_MASK);
}



extern "C"
void unmask_interrupts(){
	outb(PIC_MASTER_DATA_PORT,
		 FULL_MASK
		 & UNMASK_KEYBOARD_INTERRUPT
		 & UNMASK_TIMER_INTERRUPT
		 & UNMASK_SLAVE_INTERRUPT);
	outb(PIC_SLAVE_DATA_PORT, FULL_MASK & UNMASK_RTC_INTERRUPT);
}
