#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include"stages.h"
#include"structs.h"
#include"cache.h"


int main (){
    // heap structs
    initStructs();
    initCache();

    // init values
    *PC = 0;
    *CLK = 0;
    iMem[0] = OR_INST;
    regFile[0x11] = 0xDEAD;
    regFile[0x12] = 0xBEEF;
    //hazards();

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
