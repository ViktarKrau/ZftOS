#include <multiboot.h>
#include "zftdef.h"
#include "zft_memory.h"
#include "string.h"
#include "terminal.h++"
#include "enterqueue.h++"
#include "inputstream.h++"



#ifndef KERNEL_H
#define KERNEL_H
class Kernel{
public:
	Kernel(multiboot_info_t* info);
	void run();
	enum ExitType{REBOOT, SHUTDOWN};
	static void exit(ExitType type);
	~Kernel();
	static Terminal* out;
	static InputStream* in;
private:
	Terminal __out;
	InputStream __in;
	uint64_t memoryUpper;
	uint64_t memoryLower;
};
#endif /*KERNEL_H*/