#include "paging.h"
#include "../zft_memory.h"





Paging* Paging::paging;



void Paging::initialize(size_t memorySize) {
    paging = new Paging(memorySize);
}



Paging::Paging(size_t memorySize)
        : pagesCount(memorySize / PAGE_SIZE), pagesMap(pagesCount) {

    usedPages = 0;
    kernelDirectory = new PageDirectory();
    memzero(kernelDirectory, sizeof(PageDirectory));
    kernelDirectory->physicalAddress = (uint32_t)kernelDirectory->tablesPhysicalAddress;
    for (uint32_t i = KERNEL_RESERVED_START_ADDRESS; i < KERNEL_RESERVED_END_ADRESS; i += PAGE_SIZE) {
        assignPage(i);
    }
}



void Paging::assignPage(uint32_t startAddres) {
    uint32_t nTable = startAddres / PAGE_SIZE;
    uint32_t tableIndex = nTable / TABLE_SIZE;
    if (kernelDirectory->tables[tableIndex]) {              //Already assigned
        return;
    }
    else {
        kernelDirectory->tables[tableIndex] = (PageTable*)startAddres;

        memzero(kernelDirectory->tables[tableIndex], PAGE_SIZE);

        kernelDirectory->tables[tableIndex] = (PageTable *)
                ((uint32_t)kernelDirectory->tables[tableIndex] & 0xFFFFF000);

        kernelDirectory->tables[tableIndex] = (PageTable *)
                ((uint32_t)kernelDirectory->tables[tableIndex] | 0x7);
        ++usedPages;
    }
}
