#include"regFile.h"
#include<stdio.h>
#include<string.h>

void initRegFile( struct MIPS_reg regFile[REGNUM]){
  // reg 0
  strcpy(regFile[0].id,"z0");
  regFile[0].val = 0x00000000;
  
  // reg 1
  strcpy(regFile[1].id,"at");
  regFile[1].val = 0x00000000;
  
  // reg 2
  strcpy(regFile[2].id,"v0");
  regFile[2].val = 0x00000000;
  // reg 3
  strcpy(regFile[3].id,"at");
  regFile[3].val = 0x00000000;

  // reg 4
  strcpy(regFile[4].id,"a0");
  regFile[4].val = 0x00000000;
  // reg 5
  strcpy(regFile[5].id,"a1");
  regFile[5].val = 0x00000000;
  // reg 6
  strcpy(regFile[6].id,"a2");
  regFile[6].val = 0x00000000;
  // reg 7
  strcpy(regFile[7].id,"a3");
  regFile[7].val = 0x00000000;

  // reg 8
  strcpy(regFile[8].id,"t0");
  regFile[8].val = 0x00000000;
  // reg 9
  strcpy(regFile[9].id,"t1");
  regFile[9].val = 0x00000000;
  // reg 10
  strcpy(regFile[10].id,"t2");
  regFile[10].val = 0x00000000;
  // reg 11
  strcpy(regFile[11].id,"t3");
  regFile[11].val = 0x00000000;
  // reg 12
  strcpy(regFile[12].id,"t4");
  regFile[12].val = 0x00000000;
  // reg 13
  strcpy(regFile[13].id,"t5");
  regFile[13].val = 0x00000000;
  // reg 14
  strcpy(regFile[14].id,"t6");
  regFile[14].val = 0x00000000;
  // reg 15
  strcpy(regFile[15].id,"t7");
  regFile[15].val = 0x00000000;

  // reg 16
  strcpy(regFile[16].id,"s0");
  regFile[16].val = 0x00000000;
  // reg 17
  strcpy(regFile[17].id,"s1");
  regFile[17].val = 0x00000000;
  // reg 18
  strcpy(regFile[18].id,"s2");
  regFile[18].val = 0x00000000;
  // reg 19
  strcpy(regFile[19].id,"s3");
  regFile[19].val = 0x00000000;
  // reg 20
  strcpy(regFile[20].id,"s4");
  regFile[20].val = 0x00000000;
  // reg 21 
  strcpy(regFile[21].id,"s5");
  regFile[21].val = 0x00000000;
  // reg 22
  strcpy(regFile[22].id,"s6");
  regFile[22].val = 0x00000000;
  // reg 23
  strcpy(regFile[23].id,"s7");
  regFile[23].val = 0x00000000;

  // reg 24
  strcpy(regFile[24].id,"t8");
  regFile[24].val = 0x00000000;
  // reg 25
  strcpy(regFile[25].id,"t9");
  regFile[25].val = 0x00000000;

  // reg 26
  strcpy(regFile[26].id,"k0");
  regFile[26].val = 0x00000000;
  // reg 27
  strcpy(regFile[27].id,"k0");
  regFile[27].val = 0x00000000;

  // reg 28
  strcpy(regFile[28].id,"gp");
  regFile[28].val = 0x00000000;

  // reg 29
  strcpy(regFile[29].id,"sp");
  regFile[29].val = 0x00000000;

  // reg 30
  strcpy(regFile[30].id,"fp");
  regFile[30].val = 0x00000000;

  // reg 31
  strcpy(regFile[31].id,"ra");
  regFile[31].val = 0x00000000;
}

