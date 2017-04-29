#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include"stages.h"
#include"structs.h"


int main (){
    // heap structs
    initStructs();

    // init values
    *PC = 0;
    *CLK = 0;
    iMem[0] = ADDI_INST;
    regFile[0x09] = 0x64;
    regFile[0x12] = 0xBEEF;
    fetch();
    shadowShift();
    decode();
    shadowShift();
    printPipe(DE);
    printCTRL(DE);
    shadowShift();
    execute();
    shadowShift();
    printPipe(EM);
    printCTRL(EM);

    // hazards();
/*
    while(*CLK < 6){
        fetch();
        decode();
        execute();
        memory();
        writeBack();
        shadowShift();
        printf("CLK: %d\n",*CLK);
        printf("PC: %d\n",*PC);
        printPipe();
        *CLK = *CLK + 1;
    }
    */
    //clear heap memory
    freeStructs();
    return 0;
}
