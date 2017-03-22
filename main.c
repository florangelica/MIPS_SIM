#include<stdio.h>
#include"regFile.h"
#include"decode.h"



int main (){
    int i;
    struct Inst *addInst = malloc(sizeof(struct Inst));
    // struct MIPS_reg regFile[REGNUM] = malloc(REGNUM * sizeof(struct regFile));
    //initRegFile(regFile);

    initInst(addInst);
    printf("addInst.op = %x\n",addInst->op);

    // add t0,s1,s2   =   0x02324020
    // op(000000)   rs(10001)   rt(10010)   rd(01000)   shamt(00000)   funct(100000)
    // 
    uint32_t add_MC = 0x02324020;

    decode( add_MC, addInst);


    free(addInst);
    return 0;
}
