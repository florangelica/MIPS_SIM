#include<stdio.h>
#include"decode.h"



int main (){
    int i;
    uint32_t *regFile;
    regFile = malloc(32*sizeof(uint32_t));
    struct Inst *addInst = malloc(sizeof(struct Inst));

    initInst(addInst);

    // add t0,s1,s2   =   0x02324020
    // op(000000)   rs(10001)   rt(10010)   rd(01000)   shamt(00000)   funct(100000)
    // 
    regFile[0] = 0x02324020;

    decode( regFile[0], addInst);


    free(regFile);
    free(addInst);
    return 0;
}
