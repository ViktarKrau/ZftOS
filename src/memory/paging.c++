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
        Page* page = getPage(i);
        allocateFrame(page, true, false);
    }
    //setDirectory(kernelDirectory);
}



Page* Paging::getPage(uint32_t startAddres) {
    uint32_t nTable = startAddres / PAGE_SIZE;
    uint32_t tableIndex = nTable / TABLE_SIZE;
    pagesMap.set(tableIndex);
    if (kernelDirectory->tables[tableIndex]) {              //Already assigned
        return &kernelDirectory->tables[tableIndex]->pages[nTable % TABLE_SIZE];
    }
    else /*ASSIGN*/{
        kernelDirectory->tables[tableIndex] = (PageTable*)startAddres;

        memzero(kernelDirectory->tables[tableIndex], PAGE_SIZE);

        kernelDirectory->tables[tableIndex] = (PageTable*)
                ((uint32_t)kernelDirectory->tables[tableIndex] & 0xFFFFF000);

        kernelDirectory->tables[tableIndex] = (PageTable*)
                ((uint32_t)kernelDirectory->tables[tableIndex] | 0x7); /*present, rw, user*/
        return &kernelDirectory->tables[tableIndex]->pages[nTable % TABLE_SIZE];
    }
}



void Paging::setDirectory(PageDirectory* directory) {
    currentDirectory = directory;
    asm volatile("mov %0, %%cr3":: "r"(directory->physicalAddress));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}



void Paging::allocateFrame(Page* page, bool isWritable, bool isKernel) {
    ++usedPages;
    uint32_t index = pagesMap.firstZero();
    if (index == (uint32_t)-1) {
        return;
    }
    pagesMap.set(index);
    page->frame = index;
    page->isPresent = 1;
    page->isUser = (isKernel)? 0 : 1;
    page->isWritable = (isWritable)? 1 : 0;
}
