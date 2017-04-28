#include"structs.h"
#include<stdio.h>
#include"instDef.h"
#include"stages.h"
#define FORWARD 0

// Input: PC, iMem
// Output: sFD
void fetch(){
    // clean shadow register 
    clearPipe(sFD);
    clearCTRL(sFD);
    // get instruction from iMem
    sFD->MI = iMem[*PC];
    printf("sFD->MI: 0x%x\n", sFD->MI);
    sFD->PC =*PC;
    *PC = *PC + 1;
}
void hazards(){
#if FORWARD
    printf("forwarding enabled\n");
    // local control lines
    uint16_t forwardA=0x00;
    uint16_t forwardB=0x00;

    // set forwarding control lines
    // EX hazard
    if( (EM->CTRL.RegWrite == 1) && !(EM->rd) && (EM->rd == sDE->rs) )forwardA = 0x10;
    if( (EM->CTRL.RegWrite == 1) && !(EM->rd) && (EM->rd == sDE->rt) )forwardB = 0x10;
    // MEM hazard
    if( (MW->CTRL.RegWrite == 1) && !(MW->rd) && (MW->rd == sDE->rs) && !(EM->CTRL.RegWrite && !(EM->rd==0) && !(EM->rd == sDE->rs)))forwardA = 0x01;
    if( (MW->CTRL.RegWrite == 1) && !(MW->rd) && (MW->rd == sDE->rt) && !(EM->CTRL.RegWrite && !(EM->rd==0) && !(EM->rd == sDE->rt)))forwardB = 0x01;
    // use control lines to forward values
#else
    printf("forwarding disabled\n");
#endif
}
// Input: FD
// Output: sDE
void decode(){
    // Clean shadow register
    clearPipe(sDE);
    clearCTRL(sDE);
    // Pass Values
    // Opcode field
    sDE->op =(uint8_t) (FD->MI >> 26);

    // R-type
    if( sDE->op == 0){
        printf("R-Type \n");
        // ----- Set Pipe Fields -----
        sDE->rs    = (uint8_t) ((FD->MI >> 21) & 0x1f);
        sDE->rt    = (uint8_t) ((FD->MI >> 16) & 0x1f);
        if((sDE->op != SRL )&& (sDE->op != SLL)){
          sDE->rd    = (uint8_t) ((FD->MI >> 11) & 0x1f);
        }
        sDE->shamt = (uint8_t) ((FD->MI >> 6) & 0x1f);
        sDE->funct = (uint8_t) (FD->MI  & 0x3f);
        // set RD1 and RD2
        sDE->RD1   = (uint32_t) regFile[sDE->rs];
        sDE->RD2   = (uint32_t) regFile[sDE->rt];
        // ----- Set Control Lines -----
        sDE->CTRL.RegDst     = (uint8_t) 1;
        sDE->CTRL.RegWrite   = (uint8_t) 1;
        if(sDE->op == JR){
            sDE->CTRL.Jump   = (uint8_t) 1;
        }

    // J TYPE
    }else if((sDE->op == J) || (sDE->op == JAL) ){
        printf("J-Type\n");
        // ----- Set Pipe Fields -----
        // set target 
        sDE->addrs = (uint32_t) (FD->MI & 0x03fffffff);
        // ----- Set Control Lines -----
        sDE->CTRL.RegWrite   = (uint8_t) 1; // write register at the end
        sDE->CTRL.Jump       = (uint8_t) 1;

    //I TYPE
    }else{
        printf("I-Type\n");
        // ----- Set Pipe Fields 
        sDE->rs    = (uint8_t) ((FD->MI >> 21) & 0x1f);
        sDE->rt    = (uint8_t) ((FD->MI >> 16) & 0x1f);
        sDE->immed = (uint32_t) ((FD->MI >> 16) & 0xFFFF);
        // ----- Set Control Lines -----
        if((sDE->op == SW) ||(sDE->op == SB)|| (sDE->op == SH)){
            sDE->CTRL.MemWrite   = (uint8_t) 1;  // write to memory for stores
        }else sDE->CTRL.RegWrite = (uint8_t) 1;  // else write a register
        if(sDE->op == LW){
            sDE->CTRL.MemtoReg   = (uint8_t) 1;  // if a load write memory to register
            sDE->CTRL.MemRead    = (uint8_t) 1;  // read memory for loads
        }
        sDE->CTRL.ALUsrc         = (uint8_t) 1;  // Get immediate field
        if((sDE->op == BEQ)||(sDE->op == BGTZ)||(sDE->op == BLEZ)||(sDE->op == BLTZ)||(sDE->op == BNE)){
            sDE->CTRL.Branch     = (uint8_t) 1;  // Branch instruction
        }
    }
}

// input: DE
// output: sEM
void execute(){
    clearPipe(sEM);
    clearCTRL(sEM);
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
    sEM->CTRL.PCsrc      = DE->CTRL.PCsrc;
    sEM->CTRL.ALUsrc     = DE->CTRL.ALUsrc;
    sEM->CTRL.RegDst     = DE->CTRL.RegDst;
    sEM->CTRL.MemWrite   = DE->CTRL.MemWrite;
    sEM->CTRL.MemRead    = DE->CTRL.MemRead;
    sEM->CTRL.MemtoReg   = DE->CTRL.MemtoReg;
    sEM->CTRL.RegWrite   = DE->CTRL.RegWrite;
    sEM->CTRL.Branch     = DE->CTRL.Branch;
    sEM->CTRL.Jump       = DE->CTRL.Jump;
    // determine ALU values
    uint32_t ALU1 = DE->RD1;
    uint32_t ALU2;
    if(DE->CTRL.ALUsrc == 0){
        ALU2 = DE->RD2; 
    }else ALU2 = DE->immed;
    // determine ALU operation
    if(DE->op == 0){ // R Type
      switch(DE->funct){
        case XOR:
            printf("XOR Instruction\n");
            sEM->ALU_result = ALU1 ^ ALU2;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case ADD:
            printf("ADD Instruction\n");
            sEM->ALU_result = ALU1 | ALU2;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case ADDU:
            printf("ADDU Instruction\n");
            sEM->ALU_result = ALU1 + ALU2;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case AND:
            printf("AND Instruction\n");
            sEM->ALU_result = (ALU1 & ALU2);
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case NOR:
            printf("NOR Instruction\n");
            sEM->ALU_result = ~(ALU1 | ALU2);
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case OR:
            printf("OR Instruction\n");
            sEM->ALU_result = ALU1 | ALU2;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SLL:
            printf("SLL Instruction\n");
            sEM->ALU_result = ALU1 << DE->shamt;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SRL:
            printf("SRL Instruction\n");
            sEM->ALU_result = ALU1 >> DE->shamt;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SLT:
            printf("SLT Instruction\n");
            if(ALU1 < ALU2){
              sEM->ALU_result == 1;
            }else{
              sEM->ALU_result == 0;
            }
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SLTU:
            printf("SLTU Instruction\n");
            if(ALU1 < ALU2){
              sEM->ALU_result == 1;
            }else{
              sEM->ALU_result == 0;
            }
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SUB:
            printf("SUB Instruction\n");
            sEM->ALU_result = ALU1 - ALU2;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SUBU:
            printf("SRL Instruction\n");
            sEM->ALU_result = ALU1 - ALU2;
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
      }
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
    sMW->CTRL.RegDst     = EM->CTRL.RegDst;
    sMW->CTRL.MemWrite   = EM->CTRL.MemWrite;
    sMW->CTRL.MemRead    = EM->CTRL.MemRead;
    sMW->CTRL.MemtoReg   = EM->CTRL.MemtoReg;
    sMW->CTRL.RegWrite   = EM->CTRL.RegWrite;
    sEM->CTRL.Branch     = DE->CTRL.Branch;
    sEM->CTRL.Jump       = DE->CTRL.Jump;
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

// PRINTING FUNCTIONS
void printCTRL(struct PIPE *pipe){
    printf("---------- Print Control ----------\n");
    printf("PCsrc:    0x%x\n",    pipe->CTRL.PCsrc);
    printf("ALUsrc:   0x%x\n",    pipe->CTRL.ALUsrc);
    printf("RegDst:   0x%x\n",    pipe->CTRL.RegDst);
    printf("RegWrite: 0x%x\n",    pipe->CTRL.RegWrite);
    printf("MemRead:  0x%x\n",    pipe->CTRL.MemRead);
    printf("MemWrite: 0x%x\n",    pipe->CTRL.MemWrite);
    printf("MemtoReg: 0x%x\n",    pipe->CTRL.MemtoReg);
    printf("Branch:   0x%x\n",    pipe->CTRL.Branch);
    printf("Jump:     0x%x\n",    pipe->CTRL.Jump);
}
void printPipe(struct PIPE *pipe){
    printf("---------- Print Pipe ----------\n");
    printf("PC: 0x%x\n",          pipe->PC);
    printf("op: 0x%x\n",          pipe->op);
    printf("rs: 0x%x\n",          pipe->rs);
    printf("rt: 0x%x\n",          pipe->rt);
    printf("rd: 0x%x\n",          pipe->rd);
    printf("shamt: 0x%x\n",       pipe->shamt);
    printf("funct: 0x%x\n",       pipe->funct);
    printf("immed: 0x%x\n",       pipe->immed);
    printf("addrs: 0x%x\n",       pipe->addrs);
    printf("MI: 0x%x\n",          pipe->MI);
    printf("RD1: 0x%x\n",         pipe->RD1);
    printf("RD2: 0x%x\n",         pipe->RD2);
    printf("WD: 0x%x\n",          pipe->WD);
    printf("ALU_result: 0x%x\n",  pipe->ALU_result);
    printf("ALU_zero: 0x%x\n",    pipe->ALU_zero);
}
void printPipeLine(){
    printf("---------- Print Pipe Line ----------\n");
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
void clearPipe(struct PIPE *pipe){
    pipe->PC          = 0;
    pipe->op          = 0;
    pipe->rs          = 0;
    pipe->rt          = 0;
    pipe->rd          = 0;
    pipe->shamt       = 0;
    pipe->funct       = 0;
    pipe->immed       = 0;
    pipe->addrs       = 0;
    pipe->MI          = 0;
    pipe->RD1         = 0;
    pipe->RD2         = 0;
    pipe->WD          = 0;
    pipe->ALU_result  = 0;
    pipe->ALU_zero    = 0;
}
void clearCTRL(struct PIPE *pipe){
    pipe->CTRL.PCsrc     = 0;
    pipe->CTRL.ALUsrc    = 0;
    pipe->CTRL.RegDst    = 0;
    pipe->CTRL.RegWrite  = 0;
    pipe->CTRL.MemRead   = 0;
    pipe->CTRL.MemWrite  = 0;
    pipe->CTRL.MemtoReg  = 0;
    pipe->CTRL.Branch    = 0;
    pipe->CTRL.Jump      = 0;
}

