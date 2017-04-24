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
    iMem[0] = J_INST;
    regFile[0x11] = 0xDEAD;
    regFile[0x12] = 0xBEEF;
    fetch();
    shadowShift();
    decode();


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
