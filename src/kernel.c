#include <stddef.h>
#include <stdint.h>
#include "zft_stdo.h"
// Check if the compiler thinks if we are targeting the wrong operating system. 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
// Target check.
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
#define BOOL uint8_t
#define TRUE 0xFF
#define FALSE 0x00

void kernel_intialize(){
	terminal_initialize();
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

void kernel_main(){
	kernel_intialize();
	kputs("Hello, World!\n");
	for(int64_t j = -200;; ++j){
		kputint(j);
		kputchar(' ');
		for(uint64_t i = 0; i < 0xfffff4; ++i);
		kputsln("");
	}
}
