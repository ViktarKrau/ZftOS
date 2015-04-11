#include "kernel.h"
#include "iob.h"
#include "app/shell.h"



Terminal* Kernel::out;
InputStream* Kernel::in;
Scheduler* Kernel::scheduler;


Kernel::Kernel(multiboot_info_t* info) : __out(COLOR_RED, COLOR_LIGHT_GREY) {
	out = &__out;
	in = &__in;
	scheduler = &__scheduler;
	memoryUpper = info->mem_upper;
	memoryLower = info->mem_lower;
}



Kernel::~Kernel() {
}



void Kernel::cycleWait(uint64_t ticks) {
	for (uint64_t i = 0; i < ticks; ++i) {
	}
}



void Kernel::exit(ExitType type) {
	if (type == REBOOT) {
		out->puts("\n\t\t\t\tREBOOT....");
		uint8_t good = 0x02;
		while (good & 0x02){
			good = inb(0x64);
		}
		outb(0x64, 0xFE);
		halt();
	}
	else {
		disable_interrupts();
		out->setStatus("");
		out->setColor(TerminalColor::COLOR_RED, TerminalColor::COLOR_BLACK);
		out->clear();
		out->putsln("\n\n\n\n\n\n\n\n");
		out->puts("\n\t\t\t\t\tIt\'s now safe to turn off your PC.");
		while (true) {
			halt();
		}
	}
}



void Kernel::run() {
	Shell shell;
	shell.schedule(0);
	out->putsln("\n\n\nSHELL EXITED\n\n\n");
}
