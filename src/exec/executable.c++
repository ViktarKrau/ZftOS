#include "executable.h"
#include "../kernel.h"
#include "../output/terminalstatebuffer.h"
#include "../iob.h"
#include "registers.h"
#define DEFAULT_STACK_SIZE 0x1000



Vector<char*>* Executable::nextStartupArgs;
Executable* Executable::nextStartupTask;


/*Function, address of is in eip, when task is runned*/
void executableEntryPoint() {
    Executable* task = Executable::nextStartupTask;
    task->active = true;
    Vector<char*> args = *Executable::nextStartupArgs;
    Kernel::scheduler.switchToPrevious();
    task->run(args);
    task->active = false;
    delete[] (char*)task->initialStackStart;
    Kernel::scheduler.removeTask(task);
    Kernel::scheduler.switchToNext();
}



Executable::Executable(bool _screenNeeded) {
    pid = 0;
    isNewScreenNeeded = _screenNeeded;
    registers.eax = 0;
    registers.ebx = 0;
    registers.ecx = 0;
    registers.edx = 0;
    registers.esi = 0;
    registers.edi = 0;
    //Getting eflags for this task, which are same as eflags of caller task
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0;"
            " popfl;":"=m"(registers.eflags)::"%eax");
    registers.eip = (uint32_t)executableEntryPoint;
    //Page directory
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(registers.cr3)::"%eax");
    /*pushing arguments of executableEntryPoint into stack*/
}



/*Is not pure virtual due to some problems with vatable while compiling */
int Executable::run(Vector<char*> args) {
    Kernel::out.puts("ERROR EXECUTING");
    args.empty();
    return -1;
}



bool Executable::isActive() {
    return active;
}



int Executable::scheduleAbove(Vector<char*> args) {
    char* previousStatus = new char[81];
    Kernel::out.getStatus(previousStatus);
    TerminalStateBuffer* buffer = nullptr;
    if (isNewScreenNeeded) {
        buffer = new TerminalStateBuffer();
        Kernel::out.saveToBuffer(buffer);

    }
    int retval = run(args);
    if (buffer != nullptr) {
        Kernel::out.restoreFromBuffer(buffer);
    }
    Kernel::out.setStatus(previousStatus);
    delete previousStatus;
    return retval;
}



void Executable::printArgs(Vector<char *> &args) {
    for (auto i = args.begin(); i != args.end(); ++i) {
        Kernel::out.putsln(*i);
    }
}



pid_t Executable::getPid() {
    return pid;
}



int Executable::schedule(Vector<char*> args) {
    initialStackStart = (uint32_t) (new char[DEFAULT_STACK_SIZE]);
    registers.esp = initialStackStart + DEFAULT_STACK_SIZE;
    *nextStartupArgs = args;
    nextStartupTask = this;
    Kernel::scheduler.createTask(this);
    return 0;
}
