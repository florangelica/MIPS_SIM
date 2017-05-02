#include"mainMemory.h"
#include<stdlib.h>
#include"instDef.h"
#include<stdio.h>

uint32_t programImage1[MEM_SIZE]={ 
  0x00000fa0,//   $sp = 4000
  0x00000fa0,//   $fp = 4000
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000032,//$pc = 50
  0x00000000,//reserved for answer
  0x00000000,//reserved for answer
  0x00000000,//reserved for answer
  0x00000000,//reserved for answer
  0x00001025,     // movev0,zero    <load_arrays>:
  0x24060064,     // lia2,100
  0x8c830000,     // lwv1,0(a0)
  0x00621821,     // adduv1,v1,v0
  0xac830000,     // swv1,0(a0)
  0x8ca30000,     // lwv1,0(a1)
  0x00621821,     // adduv1,v1,v0
  0xaca30000,     // swv1,0(a1)
  0x24420001,     // addiuv0,v0,1
  0x24840004,     // addiua0,a0,4
  0x1446fff7,     // bnev0,a2,400838 <load_arrays+0x8>
  0x24a50004,     // addiua1,a1,4
  0x8d090f90,//lw $t1, 3576($t0)
  0xad090018,//sw $t1, 24($t0)
  0x8d080418,//lw $t0, 1048($t0)
  0x03e00008,     // jrra
  0xac030018,     // sw v1, 24($zer0)
  0x00000000,     // nop
  0x00000000,     // nop
  0x00000000,     // nop
  0x00801825,    //  movev1,a0<check_result>:
  0x24870190,    // addiua3,a0,400
  0x00001025,    // movev0,zero
  0x24080001,    // lit0,1
  0x8c640000,    // lwa0,0(v1)
  0x8ca60000,    // lwa2,0(a1)
  0x00862026,    // xora0,a0,a2
  0x0104100b,    // movnv0,t0,a0
  0x24630004,    // addiuv1,v1,4
  0x1467fffa,    // bnev1,a3,400878 <check_result+0x10>
  0x24a50004,    // addiua1,a1,4
  0xac02001c,//sw $v0, 28($zero)
  0x8c18041c,//lw $t8, 1052($zero)
  0xac030020,//sw $v1, 32($zero)
  0x8c180420,//lw $t8, 1056($zero)
  0x03e00008,    // jrra
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x00000000,    // nop
  0x27bdfcb0,    // addiusp,sp,-848   <main>:
  0xafbf034c,    // swra,844(sp)
  0xafb20348,    // sws2,840(sp)
  0xafb10344,    // sws1,836(sp)
  0xafb00340,    // sws0,832(sp)
  0xafa2033c,    // swv0,828(sp)
  0x27b101ac,    // addius1,sp,428
  0x02202825,    // movea1,s1
  0x27b0001c,    // addius0,sp,28
  0x02002025,    // movea0,s0
  0x0c00000a,    // jal10 <load_arrays>
  0x00000000,    // nop
  0x02202825,    // movea1,s1
  0x02002025,    // movea0,s0
  0x0c00001e,    // jal30 <check_result>
  0x00000000,    // nop
  0x8fa4033c,    // lwa0,828(sp)
  0x8e430000,    // lwv1,0(s2)
  0x10830004,    // beqa0,v1,400914 <main+0x78>
  0x8fbf034c,    // lwra,844(sp)
  0x00000000,    // nop
  0x8fb20348,    // lws2,840(sp)
  0x8fb10344,    // lws1,836(sp)
  0x8fb00340,    // lws0,832(sp)
  0x00000008,    // jr$zero
  0x00000000,   //
  0x00000000   //

};
void initMemory(){
    iMem = malloc(MEM_SIZE*sizeof(uint32_t));
    dMem = malloc(MEM_SIZE*sizeof(uint32_t));
    int i;
    for( i = 0; i < MEM_SIZE; i++){
      iMem[i] = programImage1[i];
    }
}


void freeMemory(){
    free(iMem);
    free(dMem);
}
