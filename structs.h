#ifndef STRUCTS_H_
#define STRUCTS_H_
#include<stdint.h>
#include<stdlib.h>

//register values
#define $zero 0
#define $at   1
#define $v0   2
#define $v1   3
#define $a0   4
#define $a1   5
#define $a2   6
#define $a3   7
#define $t0   8
#define $t1   9
#define $t2   10
#define $t3   11
#define $t4   12
#define $t5   13
#define $t6   14
#define $t7   15
#define $s0   16
#define $s1   17
#define $s2   18
#define $s3   19
#define $s4   20
#define $s5   21
#define $s6   22
#define $s7   23
#define $t8   24
#define $t9   25
#define $k0   26
#define $k1   27
#define $gp   28
#define $sp   29
#define $fp   30
#define $ra   31

struct CONTROL{
    // 0: next PC = PC +4        1: computed branch target 
    uint8_t PCsrc: 1;
    // 0: 2nd ALU operand = RD2  1: 2nd ALU Operand = 16-bit sign extnd immed val 
    uint8_t ALUsrc: 1; 
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
    // 0: Not a Branch           1: Branch Instruciton
    uint8_t Branch: 1;
    // 0: Not a Jump             1: Jump Instruciton
    uint8_t Jump: 1;

};

struct PIPE{
    uint32_t pc: 32;
    uint8_t op: 6;
    uint8_t rs: 5;
    uint8_t rt: 5;
    uint8_t rd: 5;
    uint8_t shamt: 5;
    uint8_t funct: 6;
    int32_t immed: 32;
    uint32_t addrs: 26;
    uint32_t MI: 32;
    int32_t RD1: 32;
    int32_t RD2: 32;
    uint32_t RD: 32;
    uint32_t WD: 32;
    uint32_t ALU_result: 32;
    uint8_t ALU_zero: 1;
    struct CONTROL CTRL;
};

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
