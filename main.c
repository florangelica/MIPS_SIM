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
    iMem[0] = OR_INST;
    regFile[0x11] = 0xDEAD;
    regFile[0x12] = 0xBEEF;

    // Test stages with or instruction
    printf("BEFORE FETCH\n");
    printPipe();
    fetch();
    shadowShift();
    printf("BEFORE DECODE\n");
    printPipe();
    decode();
    shadowShift();
    printf("BEFORE EX\n");
    printPipe();
    execute();
    //memory(EM, sMW);
    //writeback();

    //clear heap memory
    freeStructs();
    return 0;
}
