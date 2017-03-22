#ifndef DECODE_H_
#define DECODE_H_

#include<stdint.h>
struct Inst{
  uint8_t op: 6;
  uint8_t rs: 5;
  uint8_t rt: 5;
  uint8_t rd: 5;
  uint8_t shamt: 5;
  uint8_t funct: 6;
  uint16_t immed: 16;
  uint32_t addrs: 26;
};
  
void initInst(struct Inst* nInst);
void decode(uint32_t inst_MC, struct Inst *DE);

#endif
