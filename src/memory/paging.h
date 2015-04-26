#include "../zftdef.h"
#include "bitmap.h"


#ifndef _ZFTOS_DEV_PAGING_H_
#define _ZFTOS_DEV_PAGING_H_



#define PAGE_TABLE_SIZE 1024
#define PAGE_DIRECTORY_SIZE 1024
//page size is equal to 4 KiB
#define PAGE_SIZE 0x1000
#define TABLE_SIZE 1024
#define KERNEL_RESERVED_START_ADDRESS 0x0
//10 MiB reserved for Kernel
#define KERNEL_RESERVED_END_ADRESS (0xA << 20)



struct Page
{
    uint32_t isPresent      : 1;            // 1 = page is present in memory
    uint32_t isWritable     : 1;            // 0 = readonly; 1 = readwrite
    uint32_t isUser         : 1;            // 0 = supervisor level only
    uint32_t wasAccessed    : 1;            // 1 = the page has been accessed since last refresh
    uint32_t isDirty        : 1;            // 1 = the page has been written to since last refresh
    uint32_t unused         : 7;            // reserved or unused
    uint32_t frame          : 20;           // frame address (shifted right 12 bits)
};



struct PageTable
{
    struct Page pages[PAGE_TABLE_SIZE];
};



struct PageDirectory
{
    struct PageTable* tables[PAGE_DIRECTORY_SIZE];                    // pointers to page tables
    uint32_t tablesPhysicalAddress[PAGE_DIRECTORY_SIZE];              // pointers to physical location of pages above for cr3
    uint32_t physicalAddress;                                         //physical address of
};


class Paging {
public:
    Paging(size_t memorySize);
    static void initialize(size_t memorySize);
private:
    static Paging* paging;
    size_t pagesCount;
    Bitmap pagesMap;
    PageDirectory* kernelDirectory;
    PageDirectory* userspaceDirectory;
    PageDirectory* currentDirectory;
    Page* getPage(uint32_t startAddress);
    void allocateFrame(Page* page, bool isWritable, bool isKernel);
    void setDirectory(PageDirectory* directory);
    uint32_t usedPages;
};

void initialize_paging();



#endif //_ZFTOS_DEV_PAGING_H_
