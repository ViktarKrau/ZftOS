#include "scheduler.h"
#include "../iob.h"
#include "../kernel.h"
#include "../app/shell.h"


extern "C" void switch_tasks(Registers* from, Registers* to);
extern "C" Registers* next_task_registers_ptr;
extern "C" Registers* previous_task_registers_ptr;


void Scheduler::createTask(Executable* task) {

    disable_interrupts();
    for (uint32_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i] == nullptr) {
            tasks[i] = task;
            task->pid = (pid_t) i;
            enable_interrupts();
            return;
        }
    }
    task->pid = tasks.size();
    tasks.push_back(task);
    enable_interrupts();
    //switchToNext();
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
    uint32_t startPid = currentTask->getPid();
    uint32_t nextPid = startPid + 1;
    while (nextPid != startPid) {
        if (nextPid > tasks.size()) {
            nextPid = 0;
            continue;
        }
        if (tasks[nextPid] != nullptr) {
            return tasks[nextPid];
        }
        else {
            nextPid++;
        }
    }
    return currentTask;
}



void switchTasks() {
    if (!Kernel::scheduler.isControlPassed) {
        Kernel::scheduler.switchToNextLater();
    }
    else {
        Kernel::scheduler.isControlPassed = false;
    }
}



void Scheduler::passControl() {
    isControlPassed = true;
    switchToNext();
}



Scheduler::Scheduler(Executable* initialTask) : tasks(0) {
    createTask(initialTask);
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



void Scheduler::switchToNextLater() {
    Executable* next = getNextTask();

    if (next == currentTask || next == nullptr) {
        //No, we don't need switch
        next_task_registers_ptr = nullptr;
        return;
    }
    else {
        /*Kernel::out << (uint32_t)next << " ";
        Kernel::out << (uint32_t)currentTask;
        Kernel::out << "\n" << "TASKS SIZE: " << (uint32_t)tasks.size() << "\n";
        showTaskInfo();*/
        //dead_halt();
        Kernel::out << (uint32_t)next << " " << (uint32_t)currentTask;
        //dead_halt();
        next_task_registers_ptr = &next->registers;
        previous_task_registers_ptr = &currentTask->registers;
        currentTask = next;
        return;
    }

}



void Scheduler::showTaskInfo() {
    for (size_t i = 0; i < tasks.size(); ++i) {
        Kernel::out << (uint32_t)tasks[i] << "\n";
    }
}
