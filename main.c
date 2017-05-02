#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include"stages.h"
#include"structs.h"
#include"cache.h"
#include"mainMemory.h"

int main (){
    // heap structs 
    
    initStructs();
    initCache();
    initMemory();
    // init values
    *PC = (uint32_t) iMem[5];
    printf("PC: 0x%x\n", *PC);
    regFile[$sp] = iMem[0];
    regFile[$fp] = iMem[1];

    *CLK = 0;

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
    freeMemory();
    freeStructs();
    return 0;
}
