#include<stdio.h>
#include<stdlib.h>
#include"structs.h"


void initStructs(){
    regFile = malloc(32*sizeof(uint32_t));
    iMem = malloc(MEM_SIZE*sizeof(uint32_t));
    dMem = malloc(MEM_SIZE*sizeof(uint32_t));
    ctrl = malloc(sizeof(struct CONTROL));
    FD = malloc(sizeof(struct PIPE));
    DE= malloc(sizeof(struct PIPE));
    EM= malloc(sizeof(struct PIPE));
    MW= malloc(sizeof(struct PIPE));
    
     sFD = malloc(sizeof(struct PIPE));
     sDE= malloc(sizeof(struct PIPE));
     sEM = malloc(sizeof(struct PIPE));
     sMW = malloc(sizeof(struct PIPE));
}

void freeStructs(){
    free(regFile);
    free(iMem);
    free(dMem);
    free(ctrl);
    free(FD);
    free(DE);
    free(EM);
    free(MW);
    free(sFD);
    free(sDE);
    free(sEM);
    free(sMW);
}
