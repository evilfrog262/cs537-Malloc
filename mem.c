#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mem.h"

int m_error;

//KRISTIN
int
Mem_Init(int sizeOfRegion)
{
    printf("I'm calling : mem_init()");
    return 0;
}
//SID
void *
Mem_Alloc(int size, int style)
{
    return NULL;
}

//KRISTIN
int
Mem_Free(void *ptr)
{
    return -1;
}

//SID
void
Mem_Dump()
{
    printf("dump:\n")
}