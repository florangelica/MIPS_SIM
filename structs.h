#ifndef STRUCTS_H_
#define STRUCTS_H_
#include<stdint.h>
#include<stdlib.h>

#define MEM_SIZE 500

struct PIPE{
  uint32_t PC: 32;
  uint8_t op: 6;
  uint8_t rs: 5;
  uint8_t rt: 5;
  uint8_t rd: 5;
  uint8_t shamt: 5;
  uint8_t funct: 6;
  uint16_t immed: 16;
  uint32_t addrs: 26;
  uint32_t MI: 32;
  uint32_t RD1: 32;
  uint32_t RD2: 32;
  uint32_t WD: 32;
  uint32_t ALU_result: 32;
  uint8_t ALU_zero: 1;
};

struct CONTROL{
  uint8_t c1:1;
  uint8_t c2:1;
  uint8_t c3:1;
  uint8_t c4:1;
};

uint32_t *regFile;
uint32_t *iMem;
uint32_t *dMem;

struct PIPE *FD;
struct PIPE *DE;
struct PIPE *EM;
struct PIPE *MW;
struct PIPE *sFD;
struct PIPE *sDE;
struct PIPE *sEM;
struct PIPE *sMW;
struct CONTROL *ctrl;

void initStructs();
void freeStructs();
#endif

