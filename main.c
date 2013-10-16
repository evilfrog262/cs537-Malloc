#include <stdio.h>

#include "mem.h"

int main(int argc, char** argv) {
  //int success;
  Mem_Init(4096);
  //success = Mem_Init(-1);
  /*if (success < 0) {
    printf("Unsuccessful call to mem_init\n");
    }*/
  void *ptr = Mem_Alloc(100, 2);
  void *ptr2 = Mem_Alloc(100, 2);
  Mem_Free(ptr);
  Mem_Free(ptr2);
  //Mem_Dump();
  return 0;
}
