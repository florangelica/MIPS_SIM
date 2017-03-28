#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include"stages.h"
#include"structs.h"


int main (){
    // heap structs
    initStructs();
    // stack vars

    // add t0,s1,s2   =   0x02324020
    // op(000000)   rs(10001)   rt(10010)   rd(01000)   shamt(00000)   funct(100000)
    // 
    FD->MI = 0x02324020;

    decode();
    execute();
    //memory(EM, sMW);
    //writeback();

    freeStructs();
    return 0;
}
