#include<stdio.h>
#include<stdlib.h>
#include"mainMemory.h"
#include"structs.h"


void initStructs(){
    CLK = malloc(sizeof(uint32_t));
    PC = malloc(sizeof(uint32_t));
    regFile = malloc(32*sizeof(uint32_t));

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
    free(PC);
    free(CLK);
    free(regFile);
    free(FD);
    free(DE);
    free(EM);
    free(MW);
    free(sFD);
    free(sDE);
    free(sEM);
    free(sMW);

}

