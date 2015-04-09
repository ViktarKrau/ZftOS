#include "paging.h"


struct GDTDescr GDT[GDT_SIZE];


extern "C"  void load_gdt(uint32_t ptrs[2]);


void initialize_gdt() {
    //init_gdt





}