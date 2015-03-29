#include "memory.h"

#define MINIMUM_FREE_BLOCK_SIZE sizeof(size_t) + sizeof(size_t*)


using namespace forbidden;
Memory Memory::memory;



void* Memory::getBlock(size_t size) {
    size += sizeof(size_t);
    MemoryBlock memoryBlock = memory.getFirstBlock();
    while (memoryBlock.getSize() != 0) {
        if (memoryBlock.getSize() < size) {
            memoryBlock.beNext();
        }
        else {
            bool success = memory.takeBlock(memoryBlock, size);
            if (!success) {
                return nullptr;
            }
            return (void*)(memoryBlock.getLocation() + 1);
        }
    }
    return nullptr;
}



void Memory::freeBlock(size_t* blockLocation) {
    MemoryBlock newFreeBlock(blockLocation - 1, *(blockLocation - 1), nullptr, nullptr);
    *(blockLocation) = 0;
    memory.insert(newFreeBlock);
}



bool Memory::takeBlock(Memory::MemoryBlock block, size_t size) {
    if (block.getSize() < size) {
        return false;
    }
    else if (block.getSize() == size || block.getSize() < (size + MINIMUM_FREE_BLOCK_SIZE)) {
        block.getPrevious().setNext(block.getNext().getLocation());
        return true;
    }
    else {
        MemoryBlock nextBlock = block.makeForward(size);
        if (block.getPrevious().getSize() != 0) {
            block.getPrevious().setNext(nextBlock.getLocation());
            return true;
        }
        else {
            firstBlock = nextBlock;
            firstBlock.resetPrevious();
            return true;
        }
    }
}



Memory::MemoryBlock Memory::MemoryBlock::getNext() {
    if (next != nullptr) {
        return MemoryBlock(next, *next, location, *(size_t**)(next + 1));
    }
    else {
        return MemoryBlock(nullptr, 0, getLocation(), nullptr);
    }
}



Memory::MemoryBlock Memory::getFirstBlock() {
    return firstBlock;
}



Memory::MemoryBlock::MemoryBlock(size_t *_location, size_t _size, size_t* _previous, size_t *_next)
        : location(_location), size(_size), previous(_previous), next(_next) {
}



void Memory::MemoryBlock::setNext(size_t* _next) {
    *((size_t**)(location + 1)) = _next;
    next = _next;
}



size_t Memory::MemoryBlock::getSize() {
    return size;
}



Memory::MemoryBlock Memory::MemoryBlock::getPrevious() {
    if (previous) {
        return MemoryBlock(previous, (*previous), nullptr, location);
    }
    else {
        return MemoryBlock(nullptr, 0, nullptr, nullptr);
    }
}



size_t* Memory::MemoryBlock::getLocation() {
    return location;
}



void Memory::MemoryBlock::makeMemoryProjection() {
    *location = size;
    *((size_t**)(location + 1)) = next;
}



Memory::MemoryBlock Memory::MemoryBlock::makeForward(size_t offset) {
    MemoryBlock retval((size_t*)((byte_ptr_t)location + offset)
            , size - offset, location, next);
    retval.makeMemoryProjection();
    size -= offset;
    /*Making projection, but only it's part*/
    *location -= offset;
    return retval;
}



void Memory::insert(Memory::MemoryBlock block) {
    MemoryBlock tempBlock = firstBlock;
    while (tempBlock.getSize() != 0) {
        bool mergeSuccess = block.tryToMergeOrInsert(tempBlock);
        if (mergeSuccess) {
            return;
        }
        tempBlock.beNext();
    }
}



bool Memory::MemoryBlock::tryToMergeOrInsert(Memory::MemoryBlock block) {
    bool success = false;
    if ((byte_ptr_t)location + size == (byte_ptr_t)block.location){
        size += block.size;
        if (block.next && block.next != next){
            next = block.next;
        }
        makeMemoryProjection();
        success = true;
    }
    if ((byte_ptr_t)block.location + block.size == (byte_ptr_t)location) {
        size += block.size;
        location = block.location;
        if (block.next && block.next != next) {
            next = block.next;
        }
        makeMemoryProjection();
        return true;
    }
    else if (success) {
        return true;
    }
    else if ((byte_ptr_t)location + size > (byte_ptr_t)block.location){
        /*TODO*/
        getPrevious().setNext(block.getLocation());
        block.setNext(location);
        return true;
    }
    return false;
}




void Memory::initialize(size_t* startLocationPointer, const size_t memorySize) {
    memory.setFirstBlock(startLocationPointer);
    memory.setSize(memorySize);

}



void Memory::setFirstBlock(size_t* beginning) {
    firstBlock.setLocation(beginning);
    firstBlock.setNext(nullptr);
    firstBlock.setSize(size - (size_t)beginning);
    firstBlock.resetPrevious();
}



void Memory::setSize(size_t _size) {
    size = _size;
}



void Memory::MemoryBlock::setSize(size_t _size) {
    *location = _size;
    size = _size;
}



void Memory::MemoryBlock::setLocation(size_t *_location) {
    location = _location;
}



void Memory::MemoryBlock::resetPrevious() {
    previous = nullptr;
}



size_t Memory::getSize() {
    return memory.size;
}



void Memory::MemoryBlock::beNext() {
    previous = location;
    location = next;
    size = *location;
    next = *(size_t**)(location + 1);
}
