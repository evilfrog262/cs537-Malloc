#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mem.h"

int m_error;


typedef struct __list_t{
    int size;
    struct __list_t *next;
} list_t; // this is our list structure

#define MAX_HEADER_SIZE 16

typedef struct __header_t{
    int size;
    int magic;
}header_t; //header structure for each block of allocated memory


//head of list
list_t *head = NULL;
// number of calls made to mem_init
int callsToInit = 0;
// size of page in virtual memory
int pageSize;

//<--TO-DO-->KRISTIN
int
Mem_Init(int sizeOfRegion)
{
    rintf("I'm calling : mem_init()\n");
    // check for fail cases
    if (sizeOfRegion <= 0 || callsToInit > 0) {
      m_error = E_BAD_ARGS;
      return -1;
    }
    pageSize = getpagesize();
    printf("Page Size: %d\n", pageSize);
    callsToInit++;
    return 0;
}

//<--TO-DO-->SID
void *
Mem_Alloc(int size, int style)
{
    header_t = *hPointer = NULL;
    list_t *memPointer = NULL;
    list_t *tmp = head;
    list_t *parent = NULL;
    list_t *previousNode = NULL;
    if style == 0
    {//BESTFIT
        bestSize = tmp->size;
        while(tmp){
            if (tmp->size > size + MAX_HEADER_SIZE){
                if(tmp->size <= bestSize){
                    memPointer = tmp;
                    bestSize = tmp->size;
                    if (previousNode != NULL){
                        parent = previousNode;
                    }
                }
                
            }
            previousNode = tmp;
            tmp = tmp->next;
        }//end while
        
    }//end BESTFIT if
    else if(style == 1)
    {// WORST FIT
        bestSize = 0;
        while(tmp){
            if (tmp->size > size + MAX_HEADER_SIZE){
                if(tmp->size >= bestSize){
                    memPointer = tmp;
                    bestSize = tmp->size;
                    if(previousNode != NULL){
                        parent = previousNode;
                    }
                }
                
            }
            previousNode = tmp;
            tmp = tmp->next;
        }//end while
        
    }//END WORST FIT
    else if (style ==2)
    {// FIRST FIT
        while(tmp){
            if (tmp->size > size +MAX_HEADER_SIZE){
                memPointer = tmp;
                if(previousNode != NULL){
                    parent = previousNode;
                }
                break;
            }
            previousNode = tmp;
            tmp= tmp->next;
        }
        
    }//END FIRST FIT
    if memPointer != NULL
    {
        hPointer = memPointer;
        memPointer = hPointer + MAX_HEADER_SIZE;
        
        if parent == NULL
        {
            head = memPointer + size;
        }
        if parent != NULL
        {
            parent->next = memPointer + size;
        }
    }
    return (void *)memPointer;
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
