#include "taskingtest.h"
#include "../kernel.h"
#include "../iob.h"



TaskingTest::TaskingTest() : Executable(false) {
}



int TaskingTest::run(Vector<char*> args) {
    Executable* another;
    if (args.size() == 0) {
        another = new TaskingTest();
        Vector<char*> nargs(1);
        nargs[0] = (char*)this;
        another->schedule(nargs);
        for (uint32_t i = 0; i < 10; ++i) {
            Kernel::out << " A ";
            /*bool success = Kernel::scheduler.switchTo(another);
            if (!success) {
                Kernel::out << "ERROR";
                return -1;
            }*/
        }
    }
    else {
        for (uint32_t i = 0; i < 10; ++i) {
            Kernel::out << " B ";
            /*Kernel::scheduler.switchToNext();*/
        }
    }
    return 0;
}



TaskingTest::~TaskingTest() {

}
