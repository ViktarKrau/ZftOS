#include "../zftdef.h"
#include "executable.h"
#include "../vector.h"



#ifndef ZFTOS_DEV_SCHEDULER_H
#define ZFTOS_DEV_SCHEDULER_H



class Scheduler{
public:
    void createTask(Executable* task);
    void removeTask(Executable* task);
    bool isPresent(Executable* task);
    void switchToNextTask();
    void passControl();
private:

    friend void switchTasks();
    bool isControlPassed;
    Executable* getNextTask();
    Executable* currentTask;
    Vector<Executable*> tasks;
};



#endif //ZFTOS_DEV_SCHEDULER_H
