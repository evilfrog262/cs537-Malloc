#include <stdio.h>

#include "mem.h"

int main(int argc, char** argv) {
  printf("hello, world2\n");
  int success;
  Mem_Init(8);
  success = Mem_Init(-1);
  if (success < 0) {
    printf("Unsuccessful call to mem_init\n");
  }
  
  return 0;
}
