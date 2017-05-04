#ifndef MAINMEMORY_H_
#define MAINMEMORY_H_
#include<stdint.h>
//Purpose: setting up memory with program and vars 
// define constant values
#define MEM_SIZE 1200

// add t0,s1,s2   =   0x02324020
// op(000000)   rs(10001)   rt(10010)   rd(01000)   shamt(00000)   funct(100000)
#define ADD_INST 0x02324020 
// or t0,s1,s2   =    0x02324025
// op(000000)   rs(10001)   rt(10010)   rd(01000)   shamt(00000)   funct(100101)
#define OR_INST 0x02324025
// J target      =    0x08000064
// op(000010)   target(00 0000 0000 0000 0000 0110 0100)
#define J_INST  0x08000064
// lw t0, 0(t1)  =    0x8d280000
// op(100011)   rs(01001)   rt(01000) immed(0000 0000 0000 0000)
#define LW_INST 0x8d280000 
// sw t0, 0(t1)  =    0xad280000
// op(101011)   rs(01001)   rt(01000) immed(0000 0000 0000 0000)
#define SW_INST 0xad280000 
// addi t0, t1, -200)  =    0x2128ffc8
// op(001000)   rs(01001)   rt(01000) immed(1111 1111 1100 1000)
#define ADDI_INST 0x2128ffc8 

uint32_t *regFile;
uint32_t *iMem;
uint32_t *dMem;
uint32_t *PC;
uint32_t *CLK;

uint32_t makeRtype(uint8_t inst, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct);
uint32_t makeItype(uint8_t inst, uint8_t rs, uint8_t rt, uint16_t immed);

void initMemory();
void freeMemory();

#endif
