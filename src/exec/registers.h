#include "../zftdef.h"



#ifndef ZFTOS_DEV_REGISTERS_H
#define ZFTOS_DEV_REGISTERS_H



typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;



#endif //ZFTOS_DEV_REGISTERS_H
