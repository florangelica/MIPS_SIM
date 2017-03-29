#include"stages.h"
#include<stdio.h>


void decode(){
    // opcode 
    DE->op = (uint8_t) FD->MI >> 26;

    // R-type
    if( DE->op == 0){
        // rs 
        DE->rs = (uint8_t) (FD->MI >> 21) & 0x1f;
        // rt
        DE->rt = (uint8_t) (FD->MI >> 16) & 0x1f;
        // rd
        DE->rd = (uint8_t) (FD->MI >> 11) & 0x1f;
        // shamt
        DE->shamt = (uint8_t) (FD->MI >> 6) & 0x1f;
        // funct
        DE->funct = (uint8_t) FD->MI  & 0x3f;
    }else{
        // rs 
        DE->rs = (uint8_t) (FD->MI >> 21) & 0x1f;
        // rt
        DE->rt = (uint8_t) (FD->MI >> 16) & 0x1f;
        // immed
        DE->immed = (uint8_t) (FD->MI >> 16) & 0xFFFF;
    }


    printf("DE->op: 0x%x\n", DE->op);
    printf("DE->rs: 0x%x\n", DE->rs);
    printf("DE->rt: 0x%x\n", DE->rt);
    printf("DE->rd: 0x%x\n", DE->rd);
    printf("DE->shamt: 0x%x\n", DE->shamt);
    printf("DE->funct: 0x%x\n", DE->funct);
}

// input: DE
// output: sEM
void execute(){
  printf("running execute\n");
}

