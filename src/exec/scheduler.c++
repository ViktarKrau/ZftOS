#include "scheduler.h"
#include "../iob.h"
#include "../kernel.h"



void Scheduler::createTask(Executable* task) {
    disable_interrupts();

    uint32_t i = 0;



    for ( ; i < tasks.size(); ++i) {
        if (tasks[i] == nullptr) {
            tasks[i] = task;
            task->pid = (pid_t) i;
        }
    }
    if (i == tasks.size()) {
        task->pid = tasks.size();
        tasks.push_back(task);
    }
    //switchToNextTask();
    enable_interrupts();
}



void Scheduler::removeTask(Executable* task) {
    disable_interrupts();
    if (task->pid != 0) {
        tasks[task->pid] = nullptr;
    }
    else {
        Kernel::out.puts("ERROR");
        return;
    }
    switchToNextTask();
    enable_interrupts();
}



bool Scheduler::isPresent(Executable *task) {
    return tasks.contains(task);
}



void Scheduler::switchToNextTask() {
    Executable* task = getNextTask();
    if (task == currentTask) {
        return;
    }
    currentTask->saveContext();
    task->loadContext();
    currentTask = task;
}



Executable* Scheduler::getNextTask() {
    uint32_t position = currentTask->getPid() + 1;
    while (position < tasks.size()) {
        if (tasks[position] != nullptr) {
            return tasks[position];
        }
    }
    return tasks[0];
}



void switch_tasks_c() {
    if (!Kernel::scheduler.isControlPassed) {
        Kernel::scheduler.switchToNextTask();
    }
    else {
        Kernel::scheduler.isControlPassed = false;
    }
}



void Scheduler::passControl() {
    switchToNextTask();
    isControlPassed = true;
}



Scheduler::Scheduler() {
}



int64_t Scheduler::getLocation() {
    return (int64_t)this;
}
