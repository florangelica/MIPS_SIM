#include<stdio.h>
#include"regFile.h"

int main (){
    struct MIPS_reg regFile[REGNUM];
    int i;
    initRegFile(regFile);
    for ( i=0; i<32; i++){
        printf("regFile[%d].id= %s\n",i,regFile[i].id);
    }
    return 0;
}
