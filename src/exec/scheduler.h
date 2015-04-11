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
private:
    Vector<Executable*> tasks;
};



#endif //ZFTOS_DEV_SCHEDULER_H
