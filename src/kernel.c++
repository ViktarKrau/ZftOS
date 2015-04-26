#include "kernel.h"
#include "iob.h"
#include "app/shell.h"
#include "memory/placementnew.h"
#include "time.h"

/*2 << 20  ==*/
#define TERMINAL_START_ADDRESS (2097152)
/*(2 << 20) + sizeof(Terminal)*/
#define SCHEDULER_START_ADDRESS (2097184)
/*(2 << 20) + sizeof(Terminal) + sizeof(Scheduler)*/
#define INPUTSTREAM_START_ADDRESS (2097204)



Terminal* Kernel::__out = (Terminal*)TERMINAL_START_ADDRESS;
InputStream* Kernel::__in = (InputStream*)INPUTSTREAM_START_ADDRESS;
Scheduler* Kernel::__scheduler = (Scheduler*)SCHEDULER_START_ADDRESS;
Terminal& Kernel::out = *(Terminal*)(TERMINAL_START_ADDRESS);
InputStream& Kernel::in = *(InputStream*)(INPUTSTREAM_START_ADDRESS);
Scheduler& Kernel::scheduler = *((Scheduler*)(SCHEDULER_START_ADDRESS));



Kernel::Kernel(multiboot_info_t* info) {
	__out = new ((void*)__out) Terminal(TerminalColor::COLOR_RED, TerminalColor::COLOR_LIGHT_GREY);
	__in = new ((void*)__in) InputStream();
	__scheduler = new ((void*)__scheduler) Scheduler();


	memoryUpper = info->mem_upper;
	memoryLower = info->mem_lower;
	out.setStatus("ZFT OS");
}



Kernel::~Kernel() {
}



void Kernel::cycleWait(uint64_t ticks) {
	for (uint64_t i = 0; i < ticks; ++i) {
	}
}



void Kernel::exit(ExitType type) {
	if (type == REBOOT) {
		out.puts("\n\t\t\t\tREBOOT....");
		uint8_t good = 0x02;
		while (good & 0x02){
			good = inb(0x64);
		}
		outb(0x64, 0xFE);
		halt();
	}
	else {
		disable_interrupts();
		out.setStatus("");
		out.setColor(TerminalColor::COLOR_RED, TerminalColor::COLOR_BLACK);
		out.clear();
		out.putsln("\n\n\n\n\n\n\n\n");
		out.puts("\n\t\t\t\t\tIt\'s now safe to turn off your PC.");
		while (true) {
			halt();
		}
	}
}



void Kernel::run() {
	out.puts("RUNNING SHELL");
	Shell shell;
	shell.schedule(0);
	out.putsln("\n\n\nSHELL EXITED\n\n\n");
}



void Kernel::initializeTasking(uint32_t stack) {

}
