#ifndef MAINMEMORY_H_
#define MAINMEMORY_H_
#include<stdint.h>
//Purpose: setting up memory with program and vars 

// define constant values
#define MEM_SIZE 1200


uint32_t *regFile;
uint32_t *iMem;
uint32_t *dMem;
uint32_t *PC;
uint32_t *CLK;





void initMemory();
void freeMemory();

#endif
