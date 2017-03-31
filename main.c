#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include"stages.h"
#include"structs.h"


int main (){
    // heap structs
    initStructs();

    // init values
    FD->MI = OR_INST;
    regFile[0x11] = 0xDEAD;
    regFile[0x12] = 0xBEEF;
    
    decode();
    execute();
    //memory(EM, sMW);
    //writeback();

    //clear heap memory
    freeStructs();
    return 0;
}
