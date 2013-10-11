#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mem.h"

int m_error;

// this is our list structure
typedef struct __list_t{
    int size;
    void *next;
} list_t;

//head of list
list_t *head = NULL;

//<--TO-DO-->KRISTIN
int
Mem_Init(int sizeOfRegion)
{
    printf("I'm calling : mem_init()");
    return 0;
}

//<--TO-DO-->SID
void *
Mem_Alloc(int size, int style)
{
    
    return NULL;
}

//<--TO-DO-->KRISTIN
int
Mem_Free(void *ptr)
{
    return -1;
}

//<--TO-DO-->SID
void
Mem_Dump()
{
    printf("dump:\n");
    list_t *tmp = head;
    while (tmp) {
        printf("Free Size: %d\n",tmp->size);
        tmp = tmp->next;
    }
}