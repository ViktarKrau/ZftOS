#include "zftdef.h"

#ifndef _ZFTOS_DEV_PAGING_H_
#define _ZFTOS_DEV_PAGING_H_

#define GDT_SIZE 256
struct GDTDescr{
    uint16_t offset_lowerbits; // offset bits 0..15
    uint16_t selector;
    uint8_t zero;      // unused, set to 0
    uint8_t type_attr; // type and attributes, see below
    uint16_t offset_higherbits; // offset bits 16..31
} __attribute__((packed));

void initialize_gdt();



#endif //_ZFTOS_DEV_PAGING_H_
