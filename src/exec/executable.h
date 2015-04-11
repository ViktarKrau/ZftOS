#include "../zftdef.h"
#include "../vector.h"


#ifndef _ZFTOS_DEV_EXECUTABLE_H_
#define _ZFTOS_DEV_EXECUTABLE_H_



class Executable {
public:
    Executable(bool _screenNeeded);
    virtual int schedule(Vector<char*> args) final;
    virtual int run(Vector<char*> args);
    virtual bool isActive() final;
    virtual void printArgs(Vector<char*>& args);
private:
    bool active;
    bool isNewScreenNeeded;
};



#endif //_ZFTOS_DEV_EXECUTABLE_H_
