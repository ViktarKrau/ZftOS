#include "../exec/executable.h"



#ifndef ZFTOS_DEV_TASKINGTEST_H
#define ZFTOS_DEV_TASKINGTEST_H



class TaskingTest : public Executable {
public:
    TaskingTest();
    virtual int run(Vector<char*> args) override;
    ~TaskingTest();
};
#endif //ZFTOS_DEV_TASKINGTEST_H
