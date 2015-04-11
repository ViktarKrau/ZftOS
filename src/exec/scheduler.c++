#include "scheduler.h"
#include "../iob.h"



void Scheduler::createTask(Executable* task) {
    disable_interrupts();
    tasks.push_back(task);
    enable_interrupts();
}



void Scheduler::removeTask(Executable* task) {
    for(Vector<Executable*>::iterator i = tasks.begin(); i != tasks.end(); ++i) {
        if (task == *i) {
           tasks.remove(i);
        }
    }
}



bool Scheduler::isPresent(Executable *task) {
    return tasks.contains(task);
}
