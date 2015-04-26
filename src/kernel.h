#include <multiboot.h>
#include "zftdef.h"
#include "zft_memory.h"
#include "output/terminal.h"
#include "input/enterqueue.h"
#include "input/inputstream.h"
#include "exec/scheduler.h"



#ifndef KERNEL_H
#define KERNEL_H



class Kernel{
public:
	Kernel(multiboot_info_t* info);
	void run();
	enum ExitType{REBOOT, SHUTDOWN};
	static void exit(ExitType type);
	~Kernel();
	static Terminal& out;
	static InputStream& in;
	static Scheduler& scheduler;
	static void cycleWait(uint64_t ticks);
private:
	friend void kernel_start(multiboot_info_t* info, uint32_t stack_ptr);
	void initializeTasking(uint32_t stackPtr);
	static Scheduler* __scheduler;
	static Terminal* __out;
	static InputStream* __in;
	uint64_t memoryUpper;
	uint64_t memoryLower;
};



#endif /*KERNEL_H*/
