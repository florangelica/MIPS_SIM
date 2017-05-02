#ifndef STRUCTS_H_
#define STRUCTS_H_
#include<stdint.h>
#include<stdlib.h>

// define constant values
#define MEM_SIZE 0x55555555
// add t0,s1,s2   =   0x02324020
// op(000000)   rs(10001)   rt(10010)   rd(01000)   shamt(00000)   funct(100000)
#define ADD_INST 0x02324020 
// or t0,s1,s2   =   0x02324025
// op(000000)   rs(10001)   rt(10010)   rd(01000)   shamt(00000)   funct(100101)
#define OR_INST 0x02324025

struct CONTROL{
    // 0: next PC = PC +4        1: computed branch target 
    uint8_t PCsrc: 1;
    // 0: 2nd ALU operand = RD2  1: 2nd ALU Operand = 16-bit sign extnd immed val 
    uint8_t ALUsrc: 1; 
    // designates which operation the ALU is to execute
    uint8_t ALUop: 8;
    // 0: Destination reg = rt   1: Destination reg = rd 
    uint8_t RegDst: 1;
    // 0: NONE                   1: the WRITE REG is written to with WD
    uint8_t RegWrite: 1;
    // 0: NONE                   1: RD = Data mem from addr
    uint8_t MemRead: 1;
    // 0: NONE                   1: Data mem from addr = WD
    uint8_t MemWrite: 1;
    // 0: WD = ALU output        1: WD = from data mem
    uint8_t MemtoReg: 1;
};

struct PIPE{
    uint32_t PC: 32;
    uint8_t op: 6;
    uint8_t rs: 5;
    uint8_t rt: 5;
    uint8_t rd: 5;
    uint8_t shamt: 5;
    uint8_t funct: 6;
    uint32_t immed: 32;
    uint32_t addrs: 26;
    uint32_t MI: 32;
    uint32_t RD1: 32;
    uint32_t RD2: 32;
    uint32_t WD: 32;
    uint32_t ALU_result: 32;
    uint8_t ALU_zero: 1;
    struct CONTROL CTRL;
};

uint32_t *regFile;
uint32_t *iMem;
uint32_t *dMem;
uint32_t *PC;
uint32_t *CLK;
struct PIPE *FD;
struct PIPE *DE;
struct PIPE *EM;
struct PIPE *MW;
struct PIPE *sFD;
struct PIPE *sDE;
struct PIPE *sEM;
struct PIPE *sMW;

void initStructs();
void freeStructs();
#endif

