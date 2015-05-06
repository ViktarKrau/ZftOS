#include "executable.h"
#include "../kernel.h"
#include "../output/terminalstatebuffer.h"
#include "../iob.h"
#include "registers.h"
#define DEFAULT_STACK_SIZE 0x1000



/*Function, address of is in eip, when task is runned*/
void executableEntryPoint(Vector<char*> args, Executable* executable) {
    executable->run(args);
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
    registers.eflags = 0;
    registers.eip = (uint32_t)executableEntryPoint;
    //Page directory
    registers.cr3 = 0;
    registers.esp = (uint32_t) (new char[DEFAULT_STACK_SIZE] + DEFAULT_STACK_SIZE);
    registers.esp -= sizeof this;
    *(Executable**)registers.esp = this;
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



int Executable::schedule(Vector<char*> args) {
    registers.esp -= sizeof args;
    *(Vector<char*>*)registers.esp = args;

    char* previousStatus = new char[81];

    Kernel::out.getStatus(previousStatus);
    TerminalStateBuffer* buffer = nullptr;

    if (isNewScreenNeeded) {
        buffer = new TerminalStateBuffer();
        Kernel::out.saveToBuffer(buffer);

    }
    Kernel::out.putsln("\nSEND TO SCHEDULER TO CREATE TASK");
    Kernel::scheduler.createTask(this);
    Kernel::out.putsln("TASK CREATED");
    active = true;
    Kernel::out << "CALLED RUN\n";
    /*TODO: REMOVE*/
    int retval = run(args);
    active = false;
    //Kernel::scheduler->removeTask(this);
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


