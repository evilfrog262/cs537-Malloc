#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mem.h"

int m_error;

// NOTE: export LD_LIBRARY_PATH="./"

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
    printf("I'm calling : mem_init()\n");

    // CHECK FOR FAILURE CASES
    if (sizeOfRegion <= 0 || callsToInit > 0) {
      m_error = E_BAD_ARGS;
      return -1;
    }

    // CALCULATE SIZE OF REGION
    pageSize = getpagesize();
    printf("Page Size: %d\n", pageSize);
    // make sure region is evenly divisible by page size
    if ( (sizeOfRegion % pageSize) != 0) {
      sizeOfRegion += (pageSize - (sizeOfRegion % pageSize));
    }
    printf("Size of Region: %d\n", sizeOfRegion);
    
<<<<<<< HEAD
    int fd = open("/dev/zero", O_RDWR | O_CREAT);


    head = mmap(NULL,sizeOfRegion,PROT_READ |PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1 ,0);
=======

    // CALL TO MMAP AND INITIALIZE LIST HEADER
    int fd = open("/dev/zero", O_RDWR);
    head = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
>>>>>>> 65017e728b95660932e0cb34cf6d6148ca3fcf6f
    if (head == MAP_FAILED) {
      perror("mmap");
      exit(1);
    }
    head->size = sizeOfRegion;
    head->next = NULL;
    close(fd);


    callsToInit++;
    return 0;
}

//<--TO-DO-->SID
void *
Mem_Alloc(int size, int style)
{
    header_t *hPointer = NULL;
    list_t *memPointer = NULL;
    list_t *tmp = head;
    list_t *parent = NULL;
    list_t *previousNode = NULL;
    int bestSize;
    if(style == 0)
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
            if (tmp->size > size + MAX_HEADER_SIZE){
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
    if(memPointer != NULL)
    {
        hPointer = (void *)memPointer;
        memPointer = (void*)hPointer + MAX_HEADER_SIZE;
        
        if(parent == NULL)
        {
            head = memPointer + size;
        }
        if(parent != NULL)
        {
            parent->next = memPointer + size;
        }
    }
    return (void *)memPointer;
  return 0;
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
