#include "scheduler.h"
#include "../iob.h"
#include "../kernel.h"
#include "../app/shell.h"


extern "C" void switch_tasks(Registers* from, Registers* to);



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
    //switchToNext();
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
    switchToNext();
    enable_interrupts();
}



bool Scheduler::isPresent(Executable* task) {
    return task != nullptr && tasks[task->getPid()] == task;
}



void Scheduler::switchToNext() {
    Executable* task = getNextTask();
    if (task == currentTask) {
        return;
    }
    switchTo(task);
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
        Kernel::scheduler.switchToNext();
    }
    else {
        Kernel::scheduler.isControlPassed = false;
    }
}



void Scheduler::passControl() {
    isControlPassed = true;
    switchToNext();
}



Scheduler::Scheduler(Executable* initialTask) : tasks(1, initialTask) {
    //Initial task
    currentTask = initialTask;
}



int64_t Scheduler::getLocation() {
    return (int64_t)this;
}



bool Scheduler::switchTo(Executable* task) {
    if (isPresent(task)) {
        //Can't switch directly to new task, because currentTask variable
        //must be reassigned before actual task switch
        Executable* previous = currentTask;
        currentTask = task;
        switch_tasks(&previous->registers, &task->registers);
        return true;
    }
    else {
        return false;
    }
}



bool Scheduler::switchToPrevious() {
    uint32_t currentPid = currentTask->getPid();
    uint32_t previousPid;
    if (currentPid == 0) {
        previousPid = tasks.size();
    }
    else {
        previousPid = currentPid - 1;
    }
    while (tasks[previousPid] == nullptr) {
        if (previousPid == currentPid) {
            return false;
        }
        if (previousPid != 0) {
            --previousPid;
        }
        else {
            previousPid = tasks.size() - 1;
        }
    }
    switchTo(tasks[previousPid]);
    return true;
}

