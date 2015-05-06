#ifndef INTERRUPTS_H
#define INTERRUPTS_H



#define IDT_SIZE 256



struct IDTDescr{
   uint16_t offset_lowerbits; // offset bits 0..15
   uint16_t selector; 
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_higherbits; // offset bits 16..31
} __attribute__((packed));



void timerTick();
void timeAlarm();
void cmosTimerTick();
void enter_queue_push(uint8_t keycode);
extern "C" void initialize_idt();
extern "C" void keyboard_handler_c();
extern "C" void unmask_interrupts();
extern "C" void initialize_timer();
extern "C" void make_idt_entry (struct IDTDescr* descr, void (*handler)(), uint16_t selector, uint8_t type_attr);
extern struct IDTDescr IDT[IDT_SIZE];
extern "C" void get_cs();
extern "C"	uint32_t __cs;



#endif /*INTERRUPTS_H*/
