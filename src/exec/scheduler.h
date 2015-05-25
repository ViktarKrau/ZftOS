#include "../zftdef.h"
#include "executable.h"
#include "../vector.h"


#ifndef ZFTOS_DEV_SCHEDULER_H
#define ZFTOS_DEV_SCHEDULER_H



class Scheduler{
public:
    Scheduler(Executable* initialTask);
    void createTask(Executable* task);
    void removeTask(Executable* task);
    bool switchTo(Executable* task);
    bool switchToPrevious();
    bool isPresent(Executable* task);
    void switchToNext();
    void passControl();
    void showTaskInfo();
    int64_t getLocation();
private:
    friend void switchTasks();
    void switchToNextLater();
    bool isControlPassed;
    Executable* getNextTask();
    Executable* currentTask;
    Vector<Executable*> tasks;
};



#endif //ZFTOS_DEV_SCHEDULER_H
