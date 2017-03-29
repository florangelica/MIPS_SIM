#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include"stages.h"
#include"structs.h"


int main (){
    // heap structs
    initStructs();
    // stack vars

    FD->MI = OR_INST;

    decode();
    execute();
    //memory(EM, sMW);
    //writeback();

    //clear heap memory
    freeStructs();
    return 0;
}
