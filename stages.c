#include"stages.h"
#include<stdio.h>


void decode(){
    // opcode 
    DE->op = (uint8_t) FD->MI >> 26;

    // R-type
    if( DE->op == 0){
        DE->rs = (uint8_t) (FD->MI >> 21) & 0x1f;
        DE->rt = (uint8_t) (FD->MI >> 16) & 0x1f;
        DE->rd = (uint8_t) (FD->MI >> 11) & 0x1f;
        DE->shamt = (uint8_t) (FD->MI >> 6) & 0x1f;
        DE->funct = (uint8_t) FD->MI  & 0x3f;
        DE->immed = (uint32_t) 0;
        DE->addrs = (uint32_t) 0;
        DE->MI = (uint32_t) 0;
        DE->WD = (uint32_t) 0;
        DE->ALU_result = (uint32_t) 0;
        DE->ALU_zero = (uint32_t) 0;
        // set RD1 and RD2
        DE->RD1 = regFile[DE->rs];
        DE->RD2 = regFile[DE->rt];
        // set ALUsrc and ALUop based on instruction
        if(DE->funct == 0x25){
            //or
            DE->CTRL.ALUsrc = 0;
            DE->CTRL.ALUop = 0x01;
        }else if( DE->funct == 0x20){
            // and
            DE->CTRL.ALUsrc = 0;
            DE->CTRL.ALUop = 0x02;
        }
    }else{
        // rs 
        DE->rs = (uint8_t) (FD->MI >> 21) & 0x1f;
        // rt
        DE->rt = (uint8_t) (FD->MI >> 16) & 0x1f;
        // immed
        DE->immed = (uint32_t) (FD->MI >> 16) & 0xFFFF;
        DE->CTRL.ALUsrc = 1;
    }


    printf("DE->op: 0x%x\n", DE->op);
    printf("DE->rs: 0x%x\n", DE->rs);
    printf("DE->rt: 0x%x\n", DE->rt);
    printf("DE->rd: 0x%x\n", DE->rd);
    printf("DE->shamt: 0x%x\n", DE->shamt);
    printf("DE->funct: 0x%x\n", DE->funct);
    printf("DE->RD1: 0x%x\n", DE->RD1);
    printf("DE->RD2: 0x%x\n", DE->RD2);
}

// input: DE
// output: sEM
void execute(){
    printf("start of execute\n");
    // determine ALU values
    uint32_t ALU1 = DE->RD1;
    uint32_t ALU2;
    if(DE->CTRL.ALUsrc == 0){
        ALU2 = DE->RD2; 
    }else ALU2 = DE->immed;

    // determine ALU operation
    switch(DE->CTRL.ALUop){
        case 0x01:
            printf("ALUop is 0x01\n");
            break;
        case 0x02:
            printf("ALUop is 0x02\n");
            break;
    }
}

