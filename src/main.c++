#include <multiboot.h>
#include "zftdef.h"
#include "kernel.h"
#include "iob.h"
#include "interrupts.h"
#include "time.h"
#include "memory.h"

#define MEMORY_START (2 << 20)

void kernel_start(multiboot_info_t* info){
	disable_interrupts();
	initialize_idt();
	unmask_interrupts();
	initialize_timer();
	enable_interrupts();
	Time::initialize();
	forbidden::Memory::initialize((size_t*)MEMORY_START, info->mem_upper);
	Kernel kernel(info);
	kernel.run();

}



//Use C linkage for kernel_main.
#if defined(__cplusplus)
extern "C" 

#endif

void kernel_main(multiboot_info_t* mbd){
	kernel_start(mbd);

}
