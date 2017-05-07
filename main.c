#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include"stages.h"
#include"structs.h"
#include"cache.h"
#include"memory.h"
#include"instDef.h"

int main (){
    // heap structs 

    initStructs();
    initCache();
    initMemory();
    // init values
    *PC = (uint32_t) iMem[5];
    printf("PC: 0x%x\n", *PC);
    printf("iMem[*PC]: 0x%x\n", iMem[*PC]);
    regFile[$sp] = iMem[0];
    regFile[$fp] = iMem[1];
    *CLK = 0;
/*
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
*/
    int i = 0;
    while(*PC != 0){
      if(i ==260){
          printf("i is 260");
      }
      if(*PC == 209){
        printf("jump to end of program  \n");
      }
      if(*PC == 150){
        printf("copy array");
        i =0;
      }
      if(*PC == 30){
        printf("bubble sort");
      }
      if(*PC == 70){
        printf("insertion sort");
      }
      if(*PC == 120){
        printf("check_result");
      }
        fetch();
//        printPipe(FD);
        writeBack();
        decode();
//        printPipe(DE);
        execute();
//        printPipe(EM);
        memory();
        hazards();
        shadowShift();
//        printPipe(MW);
        *CLK = *CLK + 1;
        i++;
    }
    //clear heap memory
    freeMemory();
    freeStructs();
    return 0;
}
