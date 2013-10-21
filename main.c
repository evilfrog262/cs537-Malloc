#include <stdio.h>

#include "mem.h"
#include "mem.c"

int main(int argc, char** argv) {
    
    Mem_Init(4096);
    
    Mem_Dump();
    void *ptr = Mem_Alloc(100, 0);
    void *ptr2 = Mem_Alloc(100, 0);
    void *ptr3 = Mem_Alloc(100, 0);
    void *ptr4 = Mem_Alloc(100, 0);
    Mem_Dump();
    Mem_Free(ptr2);
    Mem_Dump();
    Mem_Free(ptr3);
    Mem_Dump();
    Mem_Free(ptr4);
    Mem_Free(ptr);
    Mem_Dump();
    return 0;
}
