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
    *PC = 0x0e; //(uint32_t) iMem[5];
    printf("PC: 0x%x\n", *PC);
    regFile[$sp] = iMem[0];
    regFile[$fp] = iMem[1];
    regFile[$v0] = 0x0e;
    *CLK = 0;

    printf("FETCH STAGE\n");
    fetch();
    shadowShift();
    printPipe(FD);
    printCTRL(FD);
    printf("DECODE STAGE\n");
    decode();
    shadowShift();
    printPipe(DE);
    printCTRL(DE);
    printf("EXECUTE STAGE\n");
    execute();
    shadowShift();
    printPipe(EM);
    printCTRL(EM);
    printf("MEMORY STAGE\n");
    memory();
    shadowShift();
    printPipe(MW);
    printCTRL(MW);
    printf("WRITEBACK STAGE\n");
    writeBack();

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
