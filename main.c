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

    //Icache[0].tag = OR_INST;
    //printf("or instruction: %x\n",OR_INST);
    //printf("or instruction tag: %x\n",Icache[0].tag);
    iMem[0x04087636] = OR_INST;
    dMem[0x04087636] = OR_INST;
    mem2pipe(0x04087636,1);
    printf("-------------\n");
    mem2pipe(0x04087636,0);


    // init values
    *PC = 0;
    *CLK = 0;

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
