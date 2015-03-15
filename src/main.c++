#include <multiboot.h>
#include "kernel.h++"
#include "zftdef.h"
#include "string.h"
#include "zft_memory.h"
#include "terminal.h++"
#include "interrupts.h"
void kernel_start(multiboot_info_t* info){
	init_idt();
	enable_kb();
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