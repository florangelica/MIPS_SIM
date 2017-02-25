#ifndef REGFILE_H_
#define REGFILE_H_

#include<stdint.h>
//  There are 32 registers so you need 5 bits to address
//  Each register is 32 bits in size
//  Registers are called $0 through $31

#define REGSIZE 32
#define REGNUM  32
struct MIPS_reg{
  char id[2];
  uint32_t val : REGSIZE;
};

void initRegFile(struct MIPS_reg regFile[REGNUM]);

#endif
