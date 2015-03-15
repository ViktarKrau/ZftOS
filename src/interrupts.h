#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define IDT_SIZE 256
struct IDTDescr{
   uint16_t offset_lowerbits; // offset bits 0..15
   uint16_t selector; 
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_higherbits; // offset bits 16..31
}__attribute__((packed));

extern "C" void init_idt();
extern "C" void keyboard_handler_c();
extern "C" void enable_kb();
#endif /*INTERRUPTS_H*/