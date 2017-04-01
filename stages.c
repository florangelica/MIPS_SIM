#include"stages.h"
#include<stdio.h>

// Input: PC, iMem
// Output: sFD
void fetch(){
    // get instruction from iMem
    sFD->MI = iMem[*PC];
}

// Input: FD
// Output: sDE
void decode(){
    // opcode 
    sDE->MI = FD->MI;
    sDE->op = (uint8_t) FD->MI >> 26;

    // R-type
    if( sDE->op == 0){
        sDE->rs = (uint8_t) (FD->MI >> 21) & 0x1f;
        sDE->rt = (uint8_t) (FD->MI >> 16) & 0x1f;
        sDE->rd = (uint8_t) (FD->MI >> 11) & 0x1f;
        sDE->shamt = (uint8_t) (FD->MI >> 6) & 0x1f;
        sDE->funct = (uint8_t) FD->MI  & 0x3f;
        sDE->immed = (uint32_t) 0;
        sDE->addrs = (uint32_t) 0;
        sDE->WD = (uint32_t) 0;
        sDE->ALU_result = (uint32_t) 0;
        sDE->ALU_zero = (uint32_t) 0;
        // set RD1 and RD2
        sDE->RD1 = regFile[sDE->rs];
        sDE->RD2 = regFile[sDE->rt];

        // set control lines 
        if(sDE->funct == 0x25){
            //or
            sDE->CTRL.ALUsrc     = 0;
            sDE->CTRL.ALUop      = 0x01;
            sDE->CTRL.RegDst     = 1;
            sDE->CTRL.MemWrite   = 0;
            sDE->CTRL.MemRead    = 0;
            sDE->CTRL.MemtoReg   = 0;
            sDE->CTRL.RegWrite   = 1;
        }else if( sDE->funct == 0x20){
            // and
            sDE->CTRL.ALUsrc     = 0;
            sDE->CTRL.ALUop      = 0x02;
        }
    }else{
        // rs 
        sDE->rs = (uint8_t) (FD->MI >> 21) & 0x1f;
        // rt
        sDE->rt = (uint8_t) (FD->MI >> 16) & 0x1f;
        // immed
        sDE->immed = (uint32_t) (FD->MI >> 16) & 0xFFFF;
        sDE->CTRL.ALUsrc = 1;
    }


}

// input: DE
// output: sEM
void execute(){
    // forward pipeline values to next stage
    sEM->rs              = DE->rs;
    sEM->rt              = DE->rt;
    sEM->rd              = DE->rd;
    sEM->shamt           = DE->shamt;
    sEM->funct           = DE->funct;
    sEM->immed           = DE->immed;
    sEM->addrs           = DE->addrs;
    sEM->MI              = DE->MI;
    sEM->WD              = DE->WD;
    sEM->RD1             = DE->RD1;
    sEM->RD2             = DE->RD2;
    sEM->ALU_result      = DE->ALU_result;
    sEM->ALU_zero        = DE->ALU_zero;
    // set control lines for next stage
    sEM->CTRL.ALUsrc     = DE->CTRL.ALUsrc;
    sEM->CTRL.ALUop      = DE->CTRL.ALUop;
    sEM->CTRL.RegDst     = DE->CTRL.RegDst;
    sEM->CTRL.MemWrite   = DE->CTRL.MemWrite;
    sEM->CTRL.MemRead    = DE->CTRL.MemRead;
    sEM->CTRL.MemtoReg   = DE->CTRL.MemtoReg;
    sEM->CTRL.RegWrite   = DE->CTRL.RegWrite;

    // determine ALU values
    uint32_t ALU1 = DE->RD1;
    uint32_t ALU2;
    if(DE->CTRL.ALUsrc == 0){
        ALU2 = DE->RD2; 
    }else ALU2 = DE->immed;

    // determine ALU operation
    switch(DE->CTRL.ALUop){
        case 0x01:
            printf("ALUop is 0x01: or instruction\n");
            sEM->ALU_result = ALU1 | ALU2;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            sEM->ALU_zero = 0;
            break;
        case 0x02:
            printf("ALUop is 0x02\n");
            break;
    }
}

void memory(){
    // forward pipeline values to next stage
    sMW->rs              = EM->rs;
    sMW->rt              = EM->rt;
    sMW->rd              = EM->rd;
    sMW->shamt           = EM->shamt;
    sMW->funct           = EM->funct;
    sMW->immed           = EM->immed;
    sMW->addrs           = EM->addrs;
    sMW->MI              = EM->MI;
    sMW->WD              = EM->WD;
    sMW->RD1             = EM->RD1;
    sMW->RD2             = EM->RD2;
    sMW->ALU_result      = EM->ALU_result;
    sMW->ALU_zero        = EM->ALU_zero;
    // set control lines for next stage
    sMW->CTRL.ALUsrc     = EM->CTRL.ALUsrc;
    sMW->CTRL.ALUop      = EM->CTRL.ALUop;
    sMW->CTRL.RegDst     = EM->CTRL.RegDst;
    sMW->CTRL.MemWrite   = EM->CTRL.MemWrite;
    sMW->CTRL.MemRead    = EM->CTRL.MemRead;
    sMW->CTRL.MemtoReg   = EM->CTRL.MemtoReg;
    sMW->CTRL.RegWrite   = EM->CTRL.RegWrite;
    //check control lines
    if( (EM->CTRL.MemWrite == 0) && (EM->CTRL.MemRead == 0)){
        // no data mem access
        return;
    }else if((EM->CTRL.MemWrite == 1) && (EM->CTRL.MemRead == 0)){
        // write to data mem
        dMem[EM->ALU_result] = EM->RD2;
    }else if((EM->CTRL.MemWrite == 0) && (EM->CTRL.MemRead == 1)){
        // read from data mem
        sMW->WD = dMem[EM->ALU_result];
    }else {
        //(EM->CTRL.MemWrite == 1) && (EM->CTRL.MemRead == 1)
        printf("ERROR: Both MemWrite and MemRead asserted\n");
    }
}


void writeBack(){
    //check the control lines
    if( (MW->CTRL.MemtoReg == 0) && (MW->CTRL.RegDst == 0)){
        // MW->ALU_result is write data
        // MW->rt is destination reg
        regFile[MW->rt] = MW->ALU_result;
        printf("regFile[rt] = %x\n",MW->ALU_result);
    }else if( (MW->CTRL.MemtoReg == 1) && (MW->CTRL.RegDst == 0)){
        // MW->WD is the write data
        // MW->rt is destination reg
        regFile[MW->rt] = MW->WD;
        printf("regFile[rt] = %x\n",MW->WD);
    }else if( (MW->CTRL.MemtoReg == 0) && (MW->CTRL.RegDst == 1)){
        // MW->ALU_result is write data
        // MW->rd is destination reg
        regFile[MW->rd] = MW->ALU_result;
        printf("regFile[rd] = %x\n",MW->ALU_result);
    }else if( (MW->CTRL.MemtoReg == 1) && (MW->CTRL.RegDst == 1)){
        // MW->WD is the write data
        // MW->rd is destination reg
        regFile[MW->rd] = MW->WD;
        printf("regFile[rd] = %x\n",MW->WD);
    }
}

void shadowShift(){
    // FD
    FD->PC          = sFD->PC;
    FD->op          = sFD->op;
    FD->rs          = sFD->rs;
    FD->rt          = sFD->rt;
    FD->rd          = sFD->rd;
    FD->shamt       = sFD->shamt;
    FD->funct       = sFD->funct;
    FD->immed       = sFD->immed;
    FD->addrs       = sFD->addrs;
    FD->MI          = sFD->MI;
    FD->RD1         = sFD->RD1;
    FD->RD2         = sFD->RD2;
    FD->WD          = sFD->WD;
    FD->ALU_result  = sFD->ALU_result;
    FD->ALU_zero    = sFD->ALU_zero;
    FD->CTRL        = sFD->CTRL;
    // DE 
    DE->PC          = sDE->PC;
    DE->op          = sDE->op;
    DE->rs          = sDE->rs;
    DE->rt          = sDE->rt;
    DE->rd          = sDE->rd;
    DE->shamt       = sDE->shamt;
    DE->funct       = sDE->funct;
    DE->immed       = sDE->immed;
    DE->addrs       = sDE->addrs;
    DE->MI          = sDE->MI;
    DE->RD1         = sDE->RD1;
    DE->RD2         = sDE->RD2;
    DE->WD          = sDE->WD;
    DE->ALU_result  = sDE->ALU_result;
    DE->ALU_zero    = sDE->ALU_zero;
    DE->CTRL        = sDE->CTRL;
    // EM 
    EM->PC          = sEM->PC;
    EM->op          = sEM->op;
    EM->rs          = sEM->rs;
    EM->rt          = sEM->rt;
    EM->rd          = sEM->rd;
    EM->shamt       = sEM->shamt;
    EM->funct       = sEM->funct;
    EM->immed       = sEM->immed;
    EM->addrs       = sEM->addrs;
    EM->MI          = sEM->MI;
    EM->RD1         = sEM->RD1;
    EM->RD2         = sEM->RD2;
    EM->WD          = sEM->WD;
    EM->ALU_result  = sEM->ALU_result;
    EM->ALU_zero    = sEM->ALU_zero;
    EM->CTRL        = sEM->CTRL;
    // MW 
    MW->PC          = sMW->PC;
    MW->op          = sMW->op;
    MW->rs          = sMW->rs;
    MW->rt          = sMW->rt;
    MW->rd          = sMW->rd;
    MW->shamt       = sMW->shamt;
    MW->funct       = sMW->funct;
    MW->immed       = sMW->immed;
    MW->addrs       = sMW->addrs;
    MW->MI          = sMW->MI;
    MW->RD1         = sMW->RD1;
    MW->RD2         = sMW->RD2;
    MW->WD          = sMW->WD;
    MW->ALU_result  = sMW->ALU_result;
    MW->ALU_zero    = sMW->ALU_zero;
    MW->CTRL        = sMW->CTRL;
}

void printPipe(){
    printf("---------- Print Pipe ----------\n");
    // FD
    printf("FD->PC: 0x%x\n",          FD->PC);
    printf("FD->op: 0x%x\n",          FD->op);
    printf("FD->rs: 0x%x\n",          FD->rs);
    printf("FD->rt: 0x%x\n",          FD->rt);
    printf("FD->rd: 0x%x\n",          FD->rd);
    printf("FD->shamt: 0x%x\n",       FD->shamt);
    printf("FD->funct: 0x%x\n",       FD->funct);
    printf("FD->immed: 0x%x\n",       FD->immed);
    printf("FD->addrs: 0x%x\n",       FD->addrs);
    printf("FD->MI: 0x%x\n",          FD->MI);
    printf("FD->RD1: 0x%x\n",         FD->RD1);
    printf("FD->RD2: 0x%x\n",         FD->RD2);
    printf("FD->WD: 0x%x\n",          FD->WD);
    printf("FD->ALU_result: 0x%x\n",  FD->ALU_result);
    printf("FD->ALU_zero: 0x%x\n",    FD->ALU_zero);
    // DE
    printf("DE->PC: 0x%x\n",          DE->PC);
    printf("DE->op: 0x%x\n",          DE->op);
    printf("DE->rs: 0x%x\n",          DE->rs);
    printf("DE->rt: 0x%x\n",          DE->rt);
    printf("DE->rd: 0x%x\n",          DE->rd);
    printf("DE->shamt: 0x%x\n",       DE->shamt);
    printf("DE->funct: 0x%x\n",       DE->funct);
    printf("DE->immed: 0x%x\n",       DE->immed);
    printf("DE->addrs: 0x%x\n",       DE->addrs);
    printf("DE->MI: 0x%x\n",          DE->MI);
    printf("DE->RD1: 0x%x\n",         DE->RD1);
    printf("DE->RD2: 0x%x\n",         DE->RD2);
    printf("DE->WD: 0x%x\n",          DE->WD);
    printf("DE->ALU_result: 0x%x\n",  DE->ALU_result);
    printf("DE->ALU_zero: 0x%x\n",    DE->ALU_zero);
    // EM
    printf("EM->PC: 0x%x\n",          EM->PC);
    printf("EM->op: 0x%x\n",          EM->op);
    printf("EM->rs: 0x%x\n",          EM->rs);
    printf("EM->rt: 0x%x\n",          EM->rt);
    printf("EM->rd: 0x%x\n",          EM->rd);
    printf("EM->shamt: 0x%x\n",       EM->shamt);
    printf("EM->funct: 0x%x\n",       EM->funct);
    printf("EM->immed: 0x%x\n",       EM->immed);
    printf("EM->addrs: 0x%x\n",       EM->addrs);
    printf("EM->MI: 0x%x\n",          EM->MI);
    printf("EM->RD1: 0x%x\n",         EM->RD1);
    printf("EM->RD2: 0x%x\n",         EM->RD2);
    printf("EM->WD: 0x%x\n",          EM->WD);
    printf("EM->ALU_result: 0x%x\n",  EM->ALU_result);
    printf("EM->ALU_zero: 0x%x\n",    EM->ALU_zero);
    // MW
    printf("MW->PC: 0x%x\n",          MW->PC);
    printf("MW->op: 0x%x\n",          MW->op);
    printf("MW->rs: 0x%x\n",          MW->rs);
    printf("MW->rt: 0x%x\n",          MW->rt);
    printf("MW->rd: 0x%x\n",          MW->rd);
    printf("MW->shamt: 0x%x\n",       MW->shamt);
    printf("MW->funct: 0x%x\n",       MW->funct);
    printf("MW->immed: 0x%x\n",       MW->immed);
    printf("MW->addrs: 0x%x\n",       MW->addrs);
    printf("MW->MI: 0x%x\n",          MW->MI);
    printf("MW->RD1: 0x%x\n",         MW->RD1);
    printf("MW->RD2: 0x%x\n",         MW->RD2);
    printf("MW->WD: 0x%x\n",          MW->WD);
    printf("MW->ALU_result: 0x%x\n",  MW->ALU_result);
    printf("MW->ALU_zero: 0x%x\n",    MW->ALU_zero);
    printf("--------------------------------\n");
}
