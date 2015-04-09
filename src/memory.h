#include <multiboot.h>
#include "zftdef.h"



#ifndef _ZFTOS_DEV_MEMORY_H_
#define _ZFTOS_DEV_MEMORY_H_



class Memory{
public:

protected:
    static void* getBlock(size_t size);
    static void freeBlock(size_t* blockLocation);
    static void initialize(size_t* startLocationPointer, const size_t memorySize);
    static size_t getSize();
private:
    class MemoryBlock{
    public:
        MemoryBlock(){}
        MemoryBlock(size_t* _location, size_t _size, size_t* _previous, size_t* _next);
        size_t getSize();
        void setSize(size_t _size);
        void setNext(size_t* _next);
        MemoryBlock makeForward(size_t offset);
        void setLocation(size_t* _location);
        void resetPrevious();
        size_t* getLocation();
        void beNext();
        MemoryBlock getNext();
        MemoryBlock getPrevious();
        bool tryToMergeOrInsert(MemoryBlock block);
        void makeMemoryProjection();
    private:
        size_t* location;
        size_t  size;                               //Including this byte
        size_t* previous;
        size_t* next;
    };
    void setFirstBlock(size_t* beginning);
    void setSize(size_t _size);
    bool takeBlock(MemoryBlock block, size_t size);
    MemoryBlock getFirstBlock();
    void insert(MemoryBlock block);
    size_t size;
    MemoryBlock firstBlock;
    static Memory memory;

    friend void* operator new(size_t size);
    friend void operator delete(void* memory);
    friend void* operator new[](size_t size);
    friend void operator delete[](void* memory);
    friend void kernel_start(multiboot_info_t* info);
};


#endif //_ZFTOS_DEV_MEMORY_H_
