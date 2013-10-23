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
#define MAGIC 12345

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
    
    
    // CALL TO MMAP AND INITIALIZE LIST HEADER
    int fd = open("/dev/zero", O_RDWR);
    head = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
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
    //make sure memory allocated is in 8 byte chunks and if not change it
    if(size%8 != 0){
        size = size + (8 - size%8);
    }
    //printf("Allocating %d bytes\n", size);
    header_t *hPointer = NULL; //initialize head pointer
    int newSize =0;
    void *memPointer = NULL;
    list_t *tmp = head;
    int bestSize;
    if(style == 0)
    {//BESTFIT
        bestSize = 2000000000;
        while(tmp){
            if (tmp->size >= size + sizeof(header_t)){
                if(tmp->size <= bestSize){
                    memPointer = tmp;
                    bestSize = tmp->size;
                }
                
            }
            tmp = tmp->next;
        }//end while
        
    }//end BESTFIT if
    else if(style == 1)
    {// WORST FIT
        bestSize = 0;
        while(tmp){
            if (tmp->size >= size + sizeof(header_t)){
                if(tmp->size >= bestSize){
                    memPointer = tmp;
                    bestSize = tmp->size;
                }
                
            }
            tmp = tmp->next;
        }//end while
        
    }//END WORST FIT
    else if (style ==2)
    {// FIRST FIT
        while(tmp){
            if (tmp->size >= size + sizeof(header_t)){
                memPointer = tmp;
                break;
            }
            tmp= tmp->next;
        }
        
    }//END FIRST FIT
    if(memPointer != NULL)
    {
        tmp = (void *) memPointer;
        list_t *tmpNext = tmp->next; //place holder for next
        newSize = tmp->size - size - sizeof(header_t);
        memPointer = memPointer + tmp->size - size;
        hPointer = (void *)memPointer - sizeof(header_t);
        hPointer->size = size;
        hPointer->magic = MAGIC;
        tmp->size = newSize;
        tmp->next = tmpNext; //when you update size next changes so set it back
    }
    else if (memPointer == NULL)
    {
        m_error = E_NO_SPACE;
        return NULL;
    }
    printf("returned ptr from alloc: %p\n", memPointer);
    return (void *)memPointer;
}

//<--TO-DO-->KRISTIN
int
Mem_Free(void *ptr)
{
    printf("I'm calling: Mem_Free()\n");
    
    // CHECK FOR NULL POINTER
    if (ptr == NULL) {
        m_error = E_BAD_POINTER;
        return -1;
    }
    
    header_t *hptr = (void *) ptr - sizeof(header_t);
    
    if (hptr->magic != MAGIC) {
        m_error = E_BAD_POINTER;
        return -1;
    }
    
    // FIND SIZE OF FREE REGION
    int freeSize = (hptr->size) + sizeof(header_t);
    printf("Freed Region Size: %d\n", freeSize);
    
    // ADD FREED NODE TO FREE LIST
    list_t *tmp = head; // keep ref to head
    head = (void *)hptr; // make head point to new freed chunk
    head->next = tmp; // make new head point to old head
    head->size = freeSize;
    
    // COALESCING -- need to work on more
    list_t *outer = head;
    list_t *inner = head;
    list_t *innerPrev = NULL;
    list_t *outerPrev = NULL;
    void *outerDummy = NULL;
    void *innerDummy = NULL;
    int outerCount = 0;
    int innerCount = 0;
    
    while(outer) {
        outerDummy = outer;
        inner = head;
        innerCount = 0;
        
        
        while(inner) {
            innerDummy = inner;
            if (inner == outer) {
                innerPrev = inner;
                innerCount++;
                inner = inner->next;
                continue;
            }
            
            if (outerCount < innerCount) {
                printf("outerdummy:%p\t outer->sizeL:%d\t sum:%p\t innerDummy:%p\n",outerDummy,outer->size,outerDummy+outer->size,innerDummy);
                if (outerDummy + outer->size == innerDummy) {
                    printf("**************************\n");
                    printf("condition true\n");
                    outer->size += inner->size;
                    outer->next = inner->next;
                    innerPrev = outer;
                }
                else if (outerDummy == innerDummy + inner->size) {
                    inner->size += outer->size;
                    printf("////////////////////////\n");
                    if (outerPrev != NULL) {
                        outerPrev->next = inner;
                    } else {
                        outer = outer->next;
                        head = outer; // head should be new outer not inner
                        outerCount++;
                    }
                }
            }
            
            else if (outerCount > innerCount) {
                if (outerDummy + outer->size == innerDummy) {
                    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
                    outer->size += inner->size;
                    if (innerPrev != NULL) {
                        innerPrev->next = outer;
                    } else {
                        head = outer;
                    }
                }
                else if (outerDummy == innerDummy + inner->size) {
                    printf("#############################\n");
                    inner->size += outer->size;
                    inner->next = outer->next;
                }
            }
            innerCount++;
            inner = inner->next;
        }
        outerPrev = outer;
        outerCount++;
        outer = outer->next;
    }
    return 0;
}

//<--TO-DO-->SID
void
Mem_Dump()
{
    //printf("dump:\n");
    list_t *tmp = head;
    while (tmp) {
        printf("Free Size: %d\tPointer: %p\n",tmp->size, tmp);
        tmp = tmp->next;
    }
}