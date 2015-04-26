#include <multiboot.h>
#include "zftdef.h"
#include "kernel.h"
#include "iob.h"
#include "interrupts.h"
#include "time.h"
#include "memory/memory.h"
#include "memory/paging.h"



#define MEMORY_START ((2 << 20) + sizeof(Terminal) + sizeof(Scheduler) + sizeof(InputStream))



void kernel_start(multiboot_info_t* info, uint32_t stack_ptr) {
	disable_interrupts();
	initialize_idt();
	unmask_interrupts();
	initialize_timer();
	Time::initialize();
	Memory::initialize((size_t*)MEMORY_START, info->mem_upper);
	//Paging::initialize(info->mem_upper);
	Kernel kernel(info);
	kernel.initializeTasking(stack_ptr);
	enable_interrupts();
	kernel.run();

}



#if defined(__cplusplus)
extern "C"
#endif
void kernel_main(multiboot_info_t* mbd , uint32_t stack_ptr) {
	kernel_start(mbd, stack_ptr);

}
