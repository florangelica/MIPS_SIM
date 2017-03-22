#include"decode.h"
#include<stdio.h>

void initInst(struct Inst *nInst){
  nInst->op = 0x0A;
  nInst->rs = 0x01;
  nInst->rt = 0x0F;
  nInst->rd = 0x03;
  nInst->shamt = 0x04;
  nInst->funct = 0x05;
  nInst->immed = 0xDEAD;
  nInst->addrs = 0x3FFFFFF;
}

void decode(uint32_t inst_MC , struct Inst *DE){
    // opcode 
    DE->op = (uint8_t) inst_MC >> 26;

    // R-type
    if( DE->op == 0){
        // rs 
        DE->rs = (uint8_t) (inst_MC >> 21) & 0x1f;
        // rt
        DE->rt = (uint8_t) (inst_MC >> 16) & 0x1f;
        // rd
        DE->rd = (uint8_t) (inst_MC >> 11) & 0x1f;
        // shamt
        DE->shamt = (uint8_t) (inst_MC >> 6) & 0x1f;
        // funct
        DE->funct = (uint8_t) inst_MC  & 0x3f;
    }else{
        // rs 
        DE->rs = (uint8_t) (inst_MC >> 21) & 0x1f;
        // rt
        DE->rt = (uint8_t) (inst_MC >> 16) & 0x1f;
        // immed
        DE->immed = (uint8_t) (inst_MC >> 16) & 0xFFFF;
    }


    printf("DE->op: 0x%x\n", DE->op);
    printf("DE->rs: 0x%x\n", DE->rs);
    printf("DE->rt: 0x%x\n", DE->rt);
    printf("DE->rd: 0x%x\n", DE->rd);
    printf("DE->shamt: 0x%x\n", DE->shamt);
    printf("DE->funct: 0x%x\n", DE->funct);
}
