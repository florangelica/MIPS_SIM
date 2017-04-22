#include "cache.h"
#include<stdlib.h>

void initCache(){
    Icache = malloc(I_CS * sizeof(struct CACHE));
    Dcache = malloc(I_CS * sizeof(struct CACHE));
}
void freeCache(){
    free(Icache);
    free(Dcache);
}
