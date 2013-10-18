#include <stdio.h>

#include "mem.h"
#include "mem.c"

int main(int argc, char** argv) {
    
    Mem_Init(4096);
    
    Mem_Dump();
    void *ptr = Mem_Alloc(100, 2);
    printf("ptr: %p\n",ptr);
    Mem_Dump();
    //void *ptr2 = Mem_Alloc(100, 2);
    //printf("ptr2: %p\n",ptr2);
    //Mem_Dump();
    Mem_Free(ptr);
    Mem_Dump();
    //Mem_Free(ptr2);
    //Mem_Dump();
    return 0;
}
