#include"structs.h"
#include<stdio.h>
#include"instDef.h"
#include"memory.h"
#include"stages.h"
#include"cache.h"


// Input: PC, iMem
// Output: sFD
void fetch(){
    // clean shadow register 
    clearPipe(sFD);
    clearCTRL(sFD);
    // get instruction from iMem
    mem2pipe(1,*PC);
    printf("FETCH: *PC = %d, sFD->MI = 0x%x: ",*PC,  sFD->MI);
    sFD->pc =*PC;
}
void hazards(){
    // local control lines
    // set forwarding control lines
    if((sDE->CTRL.Branch) || (sDE->MI == NOP))return;
    uint8_t emDest, mwDest;
    if (sEM->CTRL.RegDst == 1){
        emDest=sEM->rd;
    }else{
        emDest=sEM->rt;
    }
    if (sMW->CTRL.RegDst == 1){
        mwDest=sMW->rd;
    }else{
        mwDest=sMW->rt;
    }
    
    // EX hazard
    if( (sEM->CTRL.RegWrite == 1) && !(emDest==0) && (emDest == sDE->rs) ){
        if(sEM->op == LW){
            printf("insert a bubble\n");
            // insert a bubble
            *PC = sFD->pc;
            sFD->MI = sDE->MI;
            sFD->pc = sDE->pc;
            sDE->MI = NOP;

        }else{
            printf("forward ALU_result\n");
            sDE->RD1 = sEM->ALU_result;
        }
    }
    if( (sEM->CTRL.RegWrite == 1) && !(emDest==0) && (emDest == sDE->rt) ){
        if(sEM->op == LW){
            // insert a bubble
            printf("insert a bubble\n");
            *PC = sFD->pc;
            sFD->MI = sDE->MI;
            sFD->pc = sDE->pc;
            sDE->MI = NOP;
        }else{
            printf("forward ALU_result\n");
            sDE->RD2 = sEM->ALU_result;
            if(sDE->op == SW){
              sDE->WD = regFile[sDE->RD2];
            }
        }
    }

    // MEM hazard
    if( (sMW->CTRL.RegWrite == 1) && !(mwDest==0) && (mwDest == sDE->rs) && !(sEM->CTRL.RegWrite && !(emDest==0) && !(emDest == sDE->rs))){
        // 2 options MW->RD or MW->ALU_result
        // TODO: add LB and LHW to the conditional
        if(sMW->op == LW){
            sDE->RD1 = sMW->RD;
        }else{
            sDE->RD1 = sMW->ALU_result;
        }
    }
    if( (sMW->CTRL.RegWrite == 1) && !(mwDest==0) && (mwDest == sDE->rt) && !(sEM->CTRL.RegWrite && !(emDest==0) && !(emDest == sDE->rt))){
        // 2 options MW->RD or ALU_result
        // TODO: add LB and LHW to the conditional
        if(sMW->op == LW){
            sDE->RD2 = sMW->RD;
        }else{
            sDE->RD2 = sMW->ALU_result;
            if(sDE->op == SW){
              sDE->WD = regFile[sDE->RD2];
            }
        }
    }
}

void brHazards(){
    uint8_t deDest, emDest, mwDest;
    if (DE->CTRL.RegDst == 1){
        deDest=DE->rd;
    }else{
        deDest=DE->rt;
    }
    if (EM->CTRL.RegDst == 1){
        emDest=EM->rd;
    }else{
        emDest=EM->rt;
    }
    if (MW->CTRL.RegDst == 1){
        mwDest=MW->rd;
    }else{
        mwDest=MW->rt;
    }

    // branch depends on the instruction one ahead
    if( (DE->CTRL.RegWrite == 1) && !(deDest==0) && (deDest == sDE->rs || deDest == sDE->rt) ){
        // insert a bubble
        *PC = sFD->pc;
        sFD->MI = sDE->MI;
        sFD->pc = sDE->pc;
        sDE->MI = NOP;
        return;
    }
    // branch depends on instruction two ahead
    if((EM->CTRL.RegWrite == 1) && !(emDest==0) && (emDest == sDE->rs)){
        if(EM->op == LW){
            // insert a bubble
            *PC = sFD->pc;
            sFD->MI = sDE->MI;
            sFD->pc = sDE->pc;
            sDE->MI = NOP;
            return;
        }else{
            sDE->RD1 = EM->ALU_result;
            return;
        }
    }else if((EM->CTRL.RegWrite == 1) && !(emDest==0) && (emDest == sDE->rt)){
        if(EM->op == LW){
            // insert a bubble
            *PC = sFD->pc;
            sFD->MI = sDE->MI;
            sFD->pc = sDE->pc;
            sDE->MI = NOP;
            return;
        }else{
            sDE->RD2 = EM->ALU_result;
            return;
        }
    }
    // branch depends on instruction three ahead
    if((MW->CTRL.RegWrite == 1) && !(mwDest==0) && (mwDest == sDE->rs)){
        if(MW->op == LW){
            sDE->RD1 = MW->RD;
            return;
        }
        

    }else if((EM->CTRL.RegWrite == 1) && !(emDest==0) && (emDest == sDE->rt)){
        if(MW->op == LW){
            sDE->RD2 = MW->RD;
            return;
        }
    }
}



// Input: FD
// Output: sDE
void decode(){
    printf("DECODE\n");
    // Clean shadow register
    clearPipe(sDE);
    clearCTRL(sDE);
    // Pass Values
    // forward pipeline values to next stage
    sDE->pc              = FD->pc;
    sDE->op              = FD->op;
    sDE->rs              = FD->rs;
    sDE->rt              = FD->rt;
    sDE->rd              = FD->rd;
    sDE->shamt           = FD->shamt;
    sDE->funct           = FD->funct;
    sDE->immed           = FD->immed;
    sDE->addrs           = FD->addrs;
    sDE->MI              = FD->MI;
    sDE->WD              = FD->WD;
    sDE->RD              = FD->RD;
    sDE->RD1             = FD->RD1;
    sDE->RD2             = FD->RD2;
    sDE->ALU_result      = FD->ALU_result;
    sDE->ALU_zero        = FD->ALU_zero;
    // set control lines for next stage
    sDE->CTRL.PCsrc      = FD->CTRL.PCsrc;
    sDE->CTRL.ALUsrc     = FD->CTRL.ALUsrc;
    sDE->CTRL.RegDst     = FD->CTRL.RegDst;
    sDE->CTRL.MemWrite   = FD->CTRL.MemWrite;
    sDE->CTRL.MemRead    = FD->CTRL.MemRead;
    sDE->CTRL.MemtoReg   = FD->CTRL.MemtoReg;
    sDE->CTRL.RegWrite   = FD->CTRL.RegWrite;
    sDE->CTRL.Branch     = FD->CTRL.Branch;
    sDE->CTRL.Jump       = FD->CTRL.Jump;

    if(sDE->MI == NOP){
        *PC = *PC+1;
        return;
    }else{
        sDE->op =(uint8_t) (FD->MI >> 26);
        // JUMP
        if((sDE->op == J) || (sDE->op == JAL)){
            //  printf("J-Type - op code: 0x%x\n", sDE->op);
            sDE->addrs = (uint32_t) (FD->MI & 0x03fffffff);
            *PC = ((*PC + 1)&0xf0000000)|(sDE->addrs);
            // insert bubbles
            //sFD->MI = (uint32_t) NOP;
            //clearCTRL(sFD);
            sDE->MI = (uint32_t) NOP;
            clearCTRL(sDE);
            if(sDE->op == JAL){
                regFile[$ra] = *PC + 2;
            }
            return;
            // BRANCH
        }else if((sDE->op==BEQ)||(sDE->op==BNE)||(sDE->op==BLEZ)||(sDE->op==BLTZ)||(sDE->op==BGTZ) ){
            sDE->CTRL.Branch = 1;
            sDE->ALU_zero = 0;
            sDE->rs    = (uint8_t) ((FD->MI >> 21) & 0x1f);
            sDE->rt    = (uint8_t) ((FD->MI >> 16) & 0x1f);
            sDE->immed = (uint32_t) (FD->MI & 0x0000FFFF); 
            if(0x00008000 & (sDE->immed)){
               sDE->immed += 0xffff0000;
            }
            sDE->RD1   = (uint32_t) regFile[sDE->rs];
            sDE->RD2   = (uint32_t) regFile[sDE->rt];
            brHazards();
            if(sDE->MI == NOP)return;
            switch(sDE->op){
                case BEQ:
                  if((sDE->RD1) == (sDE->RD2)){
                    sDE->ALU_zero = 1;
                  }
                  break;
                case BNE:
                  if((sDE->RD1) != (sDE->RD2)){
                    sDE->ALU_zero = 1;
                  }
                  break;
                case BLEZ:
                  if((sDE->RD1) <= 0){
                    sDE->ALU_zero = 1;
                  }
                  break;
                case BLTZ:
                  if((sDE->RD1) < 0){
                    sDE->ALU_zero = 1;
                  }
                  break;
                case BGTZ:
                  if((sDE->RD1) > 0){
                    sDE->ALU_zero = 1;
                  }
                  break;
            }
            if(sDE->ALU_zero == 1){
             // sFD->MI = (uint32_t) NOP;
             // clearCTRL(sFD);
              sDE->MI = (uint32_t) NOP;
              clearCTRL(sDE);
              *PC =((sDE->pc) +1) + (int32_t)sDE->immed;
            }else{
              *PC = *PC+1;
            }
            return;
        }else if(sDE->op == 0){
            *PC = *PC + 1;
//        printf("R-Type \n");
        // ----- Set Pipe Fields -----
            sDE->rs    = (uint8_t) ((FD->MI >> 21) & 0x1f);
            sDE->rt    = (uint8_t) ((FD->MI >> 16) & 0x1f);
            if((sDE->op != SRL )&& (sDE->op != SLL)){
                sDE->rd    = (uint8_t) ((FD->MI >> 11) & 0x1f);
            }
            sDE->shamt = (uint8_t) ((FD->MI >> 6) & 0x1f);
            sDE->funct = (uint8_t) (FD->MI  & 0x3f);
            if(sDE->funct == JR){
                // decode fields
                sDE->rs    = (uint8_t) ((FD->MI >> 21) & 0x1f);
                sDE->funct = (uint8_t) (FD->MI  & 0x3f);
                sDE->RD1 = sDE->rs;
                *PC = regFile[sDE->RD1];
                sFD->MI = (uint32_t) NOP;
                clearCTRL(sFD);
                sDE->MI = (uint32_t) NOP;
                clearCTRL(sDE);
                return;
            }
            // set RD1 and RD2
            sDE->RD1   = (uint32_t) regFile[sDE->rs];
            sDE->RD2   = (uint32_t) regFile[sDE->rt];
            // ----- Set Control Lines -----
            if((sDE->op != SLL)&&(sDE->op != SRL)){
                sDE->CTRL.RegDst     = (uint8_t) 1;
            } // SLL and SRL use rt as destination
            sDE->CTRL.RegWrite   = (uint8_t) 1;
        //I TYPE
        }else{
            *PC = *PC + 1;
//        printf("I-Type\n");
            // ----- Set Pipe Fields 
            sDE->rs    = (uint8_t) ((FD->MI >> 21) & 0x1f);
            sDE->rt    = (uint8_t) ((FD->MI >> 16) & 0x1f);
            sDE->immed = (uint32_t) (FD->MI & 0x0000FFFF); //zero extended
            if(!(sDE->op == ANDI) && !(sDE->op == ORI) && !(sDE->op == XORI)){
                if(0x00008000 & (sDE->immed)){
                   sDE->immed += 0xffff0000;
                }
            }
            sDE->RD1   = (uint32_t) regFile[sDE->rs];
            sDE->RD2   = (uint32_t) regFile[sDE->rt];
            if((sDE->op == SW) ||(sDE->op == SB)|| (sDE->op == SH)){
                sDE->WD = regFile[sDE->RD2];
            }
            // ----- Set Control Lines -----
            if((sDE->op == SW) ||(sDE->op == SB)|| (sDE->op == SH)){
                sDE->CTRL.MemWrite   = (uint8_t) 1;  // write to memory for stores
            }else sDE->CTRL.RegWrite = (uint8_t) 1;  // else write a register
            if(sDE->op == LW){
                sDE->CTRL.MemtoReg   = (uint8_t) 1;  // if a load write memory to register
                sDE->CTRL.MemRead    = (uint8_t) 1;  // read memory for loads
            }
            sDE->CTRL.ALUsrc         = (uint8_t) 1;  // Get immediate field
        }
    }
}

// input: DE
// output: sEM
void execute(){
    clearPipe(sEM);
    clearCTRL(sEM);
    // forward pipeline values to next stage
    sEM->pc              = DE->pc;
    sEM->op              = DE->op;
    sEM->rs              = DE->rs;
    sEM->rt              = DE->rt;
    sEM->rd              = DE->rd;
    sEM->shamt           = DE->shamt;
    sEM->funct           = DE->funct;
    sEM->immed           = DE->immed;
    sEM->addrs           = DE->addrs;
    sEM->MI              = DE->MI;
    sEM->WD              = DE->WD;
    sEM->RD              = DE->RD;
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
    printf("EXECUTING: *PC = %d, sEM->MI: 0x%x\n",sEM->pc,  sEM->MI);
    if(sEM->MI == NOP){
      printf("NOP INMSTRUCTION\n");
      return;
    }
    // determine ALU values
    int32_t ALU1 = sEM->RD1;
    int32_t ALU2;
    if(sEM->CTRL.ALUsrc == 0){
       ALU2 = (int32_t)sEM->RD2; 
    }else{
       ALU2 = sEM->immed;
    }
    // determineALU operation
    if(sEM->op == 0){ // R Type
      switch(sEM->funct){
        case MOVN:
            printf("MOVN Instruction\n");
            if(ALU2 != 0){
              sEM->ALU_result = ALU1; //res value will be placed in rd
            }else{
              sEM->ALU_result = regFile[sEM->rd];
            }
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case MOVZ:
            printf("MOVZ Instruction\n");
            if(ALU2 == 0){
              sEM->ALU_result = ALU1; //res value will be placed in rd
            }else{
              sEM->ALU_result = regFile[sEM->rd];
            }
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case XOR:
            break;
        case ADDU: 
            printf("ADDU Instruction\n");
            sEM->ALU_result = (uint32_t)ALU1 + (uint32_t)ALU2;
//          printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case AND:
            printf("AND Instruction\n");
            sEM->ALU_result = (ALU1 & ALU2);
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case NOR:
            printf("NOR Instruction\n");
            sEM->ALU_result = ~(ALU1 | ALU2);
            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case OR:
            printf("OR Instruction\n");
            sEM->ALU_result = ALU1 | ALU2;
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SLL:
            printf("SLL Instruction\n");
            sEM->ALU_result = ALU1 << DE->shamt;
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SRL:
            printf("SRL Instruction\n");
            sEM->ALU_result = ALU1 >> DE->shamt;
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SLT:
            printf("SLT Instruction\n");
            if(ALU1 < ALU2){
              sEM->ALU_result = 1;
            }else{
              sEM->ALU_result = 0;
            }
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SLTU:
            printf("SLTU Instruction\n");
            if(((uint32_t)ALU1) < ((uint32_t)ALU2)){
              sEM->ALU_result = 1;
            }else{
              sEM->ALU_result = 0;
            }
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SUB:
            printf("SUB Instruction\n");
            sEM->ALU_result = ALU1 - ALU2;
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
        case SUBU:
            printf("SRL Instruction\n");
            sEM->ALU_result = (uint32_t)ALU1 - (uint32_t)ALU2;
//            printf("sEM->ALU_result: %x\n",sEM->ALU_result);
            break;
      }
    }else{
        switch(sEM->op){
            case ADDI:
                printf("ADDI Instruction\n");
                sEM->ALU_result = (ALU1) + (ALU2);
//                printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
            case ADDIU:
                printf("ADDIU Instruction: ALU1:%x  ALU2: %x \n",ALU1,ALU2);
                sEM->ALU_result = (uint32_t)ALU1 + (uint32_t)ALU2;
//                printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
            case ANDI:
                printf("ANDI Instruction\n");
                sEM->ALU_result = ALU1 & ALU2;
//                printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
            case LW:
                printf("LW Instruction\n");
                sEM->ALU_result = ALU1 + ALU2;
//                printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
            case SW:
                printf("SW Instruction: ALU1:%x  ALU2: %x \n",ALU1,ALU2);
                sEM->ALU_result = (uint32_t) ALU1 + ALU2;
                printf("location calculated: 0x%x\n", sEM->ALU_result);
//                printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
            case ORI:
                printf("ORI Instruction\n");
                sEM->ALU_result = ALU1 | ALU2;
//                printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
            case XORI:
                printf("XORI Instruction\n");
                sEM->ALU_result = ALU1 ^ ALU2;
//               printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
            case SLTI:
                printf("SLTI Instruction\n");
                if(ALU1 < ALU2){
                   sEM->ALU_result == 1;
                }else{
                   sEM->ALU_result == 0;
                }
//                printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
            case SLTIU:
                printf("SLTIU Instruction\n");
                if(((uint32_t)ALU1) < ((uint32_t) ALU2)){
                   sEM->ALU_result == 1;
                }else{
                   sEM->ALU_result == 0;
                }
//               printf("sEM->ALU_result: %x\n",sEM->ALU_result);
                break;
      }
    }
//    printf("instruction sEM->pc: 0x%x\n", sEM->pc);
//    printf("actual PC: 0x%x\n", *PC);
}

void memory(){
    printf("MEMORY\n");
    clearPipe(sMW);
    clearCTRL(sMW);
    // forward pipeline values to next stage
    sMW->pc              = EM->pc;
    sMW->op              = EM->op;
    sMW->rs              = EM->rs;
    sMW->rt              = EM->rt;
    sMW->rd              = EM->rd;
    sMW->shamt           = EM->shamt;
    sMW->funct           = EM->funct;
    sMW->immed           = EM->immed;
    sMW->addrs           = EM->addrs;
    sMW->MI              = EM->MI;
    sMW->WD              = EM->WD;
    sMW->RD              = EM->RD;
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
    sMW->CTRL.Branch     = EM->CTRL.Branch;
    sMW->CTRL.Jump       = EM->CTRL.Jump;
    if(sMW->MI == NOP)return;
    //check control lines
    if((sMW->CTRL.MemWrite == 0) && (sMW->CTRL.MemRead == 0)){
        return;
    }else if((sMW->CTRL.MemWrite == 1) && (sMW->CTRL.MemRead == 0)){
      //
      // STORES: write to data mem
       int byteOffset = sMW->ALU_result & 0x00000003; 
       if(sMW->op == SW){
          printf("write to memory: 0x%x\n", sMW->WD);
            pipe2mem(sMW->ALU_result>>2, sMW->WD);
        }else{// TODO SB, SH instruction
          printf("STORE NOT IMPLEMENTED");
        }
    }else if((sMW->CTRL.MemWrite == 0) && (sMW->CTRL.MemRead == 1)){
        // LOADS: read from data mem
        int byteOffset = sMW-> ALU_result & 0x00000003;
        if(sMW->op == LW){
            mem2pipe(0,(sMW->ALU_result)>>2);
        }else{// TODO LB, LBU, LHU, LUI instruction
          printf("LOAD NOT IMPLEMENTED");
        }
    }else {
        //(EM->CTRL.MemWrite == 1) && (EM->CTRL.MemRead == 1)
        printf("ERROR: Both MemWrite and MemRead asserted\n");
    }
}

void writeBack(){
    printf("\nWRITEBACK\n");
    if(sMW->MI == NOP)return;
    //check the control lines
    if( (MW->CTRL.MemtoReg == 0) && (MW->CTRL.RegDst == 0)){
        // MW->ALU_result --> REGFILE
        // MW->rt is destination reg
        if(!(MW->CTRL.Jump)){
           regFile[MW->rt] = MW->ALU_result;
//           printf("regFile[rt] = 0x%x\n",MW->ALU_result);
        }else{
          printf("regFile[rt] = 0x%x\n", MW->ALU_result);
          int wordLocation = (MW->ALU_result) >> 2;
//          printf("wordLocation: 0x%x Instruction at jump: 0x%x\n", wordLocation, iMem[wordLocation]);
        }
    }else if( (MW->CTRL.MemtoReg == 1) && (MW->CTRL.RegDst == 0)){
        // MW->WD is the write data
        // MW->rt is destination reg
        regFile[MW->rt] = MW->RD;
//        printf("regFile[rt] = %x\n",MW->RD);
    }else if( (MW->CTRL.MemtoReg == 0) && (MW->CTRL.RegDst == 1)){
        // MW->ALU_result is write data
        // MW->rd is destination reg
        regFile[MW->rd] = MW->ALU_result;
//        printf("regFile[rd] = %x\n",MW->ALU_result);
    }else if( (MW->CTRL.MemtoReg == 1) && (MW->CTRL.RegDst == 1)){
        // MW->WD is the write data
        // MW->rd is destination reg
        regFile[MW->rd] = MW->RD;
//        printf("regFile[rd] = %x\n",MW->RD);
    }
}

void shadowShift(){
    // FD
    FD->pc          = sFD->pc;
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
    FD->RD          = sFD->RD;
    FD->ALU_result  = sFD->ALU_result;
    FD->ALU_zero    = sFD->ALU_zero;
    FD->CTRL        = sFD->CTRL;
    // DE 
    DE->pc          = sDE->pc;
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
    DE->RD          = sDE->RD;
    DE->ALU_result  = sDE->ALU_result;
    DE->ALU_zero    = sDE->ALU_zero;
    DE->CTRL        = sDE->CTRL;
    // EM 
    EM->pc          = sEM->pc;
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
    EM->RD          = sEM->RD;
    EM->ALU_result  = sEM->ALU_result;
    EM->ALU_zero    = sEM->ALU_zero;
    EM->CTRL        = sEM->CTRL;
    // MW 
    MW->pc          = sMW->pc;
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
    MW->RD          = sMW->RD;
    MW->ALU_result  = sMW->ALU_result;
    MW->ALU_zero    = sMW->ALU_zero;
    MW->CTRL        = sMW->CTRL;
}

// PRINTING FUNCTIONS
void printCTRL(struct PIPE *pipe){
    printf("---------- Print Control ----------\n");
    printf("PCsrc:    0x%x  ",    pipe->CTRL.PCsrc);
    printf("ALUsrc:   0x%x  ",    pipe->CTRL.ALUsrc);
    printf("RegDst:   0x%x  ",    pipe->CTRL.RegDst);
    printf("RegWrite: 0x%x  ",    pipe->CTRL.RegWrite);
    printf("MemRead:  0x%x  ",    pipe->CTRL.MemRead);
    printf("MemWrite: 0x%x  ",    pipe->CTRL.MemWrite);
    printf("MemtoReg: 0x%x  ",    pipe->CTRL.MemtoReg);
    printf("Branch:   0x%x  ",    pipe->CTRL.Branch);
    printf("Jump:     0x%x\n",    pipe->CTRL.Jump);
}
void printPipe(struct PIPE *pipe){
    printf("---------- Print Pipe ----------\n");
    printf("pc: 0x%x  ",          pipe->pc);
    printf("op: 0x%x  ",          pipe->op);
    printf("rs: 0x%x  ",          pipe->rs);
    printf("rt: 0x%x  ",          pipe->rt);
    printf("rd: 0x%x  ",          pipe->rd);
    printf("shamt: 0x%x  ",       pipe->shamt);
    printf("funct: 0x%x  ",       pipe->funct);
    printf("immed: 0x%x  ",       pipe->immed);
    printf("addrs: 0x%x  ",       pipe->addrs);
    printf("MI: 0x%x  ",          pipe->MI);
    printf("RD1: 0x%x  ",         pipe->RD1);
    printf("RD2: 0x%x  ",         pipe->RD2);
    printf("WD: 0x%x  ",          pipe->WD);
    printf("RD: 0x%x  ",          pipe->RD);
    printf("ALU_result: 0x%x  ",  pipe->ALU_result);
    printf("ALU_zero: 0x%x\n",    pipe->ALU_zero);
}
void printPipeLine(){
    printf("---------- Print Pipe Line ----------\n");
    // FD
    printf("FD->pc: 0x%x\n",          FD->pc);
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
    printf("FD->RD: 0x%x\n",          FD->RD);
    printf("FD->ALU_result: 0x%x\n",  FD->ALU_result);
    printf("FD->ALU_zero: 0x%x\n",    FD->ALU_zero);
    // DE
    printf("DE->pc: 0x%x\n",          DE->pc);
    printf("DE->op: 0x%x\n",          DE->op);
    printf("DE->rs: 0x%x\n",          DE->rs);
    printf("DE->rt: 0x%x\n",          DE->rt);
    printf("DE->rd: 0x%x\n",          DE->rd);
    printf("FD->ALU_result: 0x%x\n",  FD->ALU_result);
    printf("FD->ALU_zero: 0x%x\n",    FD->ALU_zero);
    // DE
    printf("DE->pc: 0x%x\n",          DE->pc);
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
    printf("DE->RD: 0x%x\n",          DE->RD);
    printf("DE->ALU_result: 0x%x\n",  DE->ALU_result);
    printf("DE->ALU_zero: 0x%x\n",    DE->ALU_zero);
    // EM
    printf("EM->pc: 0x%x\n",          EM->pc);
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
    printf("EM->RD: 0x%x\n",          EM->RD);
    printf("EM->ALU_result: 0x%x\n",  EM->ALU_result);
    printf("EM->ALU_zero: 0x%x\n",    EM->ALU_zero);
    // MW
    printf("MW->pc: 0x%x\n",          MW->pc);
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
    printf("MW->RD: 0x%x\n",          MW->RD);
    printf("MW->ALU_result: 0x%x\n",  MW->ALU_result);
    printf("MW->ALU_zero: 0x%x\n",    MW->ALU_zero);
    printf("--------------------------------\n");
}
void clearPipe(struct PIPE *pipe){
    pipe->pc          = 0;
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
    pipe->RD          = 0;
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

