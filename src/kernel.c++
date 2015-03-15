#include "kernel.h++"
#include "iob.h"
#include "interrupts.h"
#include "shell.h++"
#include "iob.h"
Terminal* Kernel::out;
InputStream* Kernel::in;
Kernel::Kernel(multiboot_info_t* info) : __out(COLOR_BLUE, COLOR_LIGHT_GREY){
	out = &__out;
	in = &__in;
	memoryUpper = info->mem_upper;
	memoryLower = info->mem_lower;
}

Kernel::~Kernel(){
}
void Kernel::exit(ExitType type){
	if(type == REBOOT){
		out->puts("\n\t\t\t\tREBOOT....");
		uint8_t good = 0x02;
		while (good & 0x02){
			good = inb(0x64);
		}
		outb(0x64, 0xFE);
		halt();
	}
	else{
		out->puts("\n\t\t\t\tSHUTDOWN....");
	}
}
extern struct IDTDescr IDT[IDT_SIZE];
void Kernel::run(){

	out->putsln("0xB8000 is equal to ");
	out->putbytes(0xB8000);
	out->putsln("\nend of terminal is equal to");
	out->putbytes(0xB8000 + 25 * 80 * 2);
	out->putsln("\nIDT begins: ");
	out->putbytes((uint32_t)IDT);
	out->putsln("\nIDT ends: ");
	out->putbytes((uint32_t)IDT + 256 * 8);
	out->putsln("\nsizeof(IDTDescr)");
	out->putbytes(sizeof(IDTDescr));
	out->putsln("\nsizeof(void*)");
	out->putint(sizeof(void*));

	out->puts("\nkernel location:  ");
	out->putbytes((uint32_t)this);
	out->puts("\nStrlen location:  ");
	out->putbytes((uint32_t)strlen);
	out->puts("\nmemduplicate location:  ");
	out->putbytes((uint32_t)memduplicate);

	out->puts("\nsizeof(int):  ");
	out->putint(sizeof(int));
	out->puts("\nsizeof(IDTDescr):   ");
	out->putint(sizeof(IDTDescr));
	Shell shell;
	shell.run();
	out->putsln("\n\n\nSHELL EXITED\n\n\n");
}

