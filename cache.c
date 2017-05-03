#include<stdlib.h>
#include<stdio.h>
#include "cache.h"
#include "structs.h"
#include "mainMemory.h"


void initCache(){
    Icache = (struct SET *) malloc(I_CS * sizeof(struct SET));
    Dcache = (struct SET *) malloc(D_CS * sizeof(struct SET));
}
void freeCache(){
    free(Icache);
    free(Dcache);
}

// TODO: add parameter to specify I or D cache
#define isHitPrint 1
int isHit(uint32_t addr, int ID){
    // hit return block number
    // miss return -1
    // ID == 0 --> Dcache

    uint32_t indx, tag, blkNum;
    indx = (addr << TAG_LEN)>>(TAG_LEN+BYO_LEN+BLO_LEN);
    tag =addr >> (IDX_LEN+BYO_LEN+BLO_LEN);
    if(ID){
        // go to index and compare tags
        for( blkNum=0; blkNum<SET_ASS; blkNum++){
            if((Icache[indx].blk[blkNum].tag == tag)&&(Icache[indx].blk[blkNum].valid )){
                #if isHitPrint
                printf("Icache: hit!\n");
                #endif
                return blkNum;
            }else{
                #if isHitPrint
                printf("Icache: miss\n");
                #endif
                return -1;
            }
        }
    }else{
        //Dcache
        // go to index and compare tags
        for( blkNum=0; blkNum<SET_ASS; blkNum++){
            if((Dcache[indx].blk[blkNum].tag == tag)&&(Dcache[indx].blk[blkNum].valid )){
                #if isHitPrint
                printf("Dcache: hit!\n");
                #endif
                return blkNum;
            }else{
                #if isHitPrint
                printf("Dcache: miss\n");
                #endif
                return -1;
            }
        }
    }
}

#define mem2cachePrint 1
void mem2cache(uint32_t addr,int ID, int blkNum){
    int i;
    uint32_t indx;
    // ID == 0 --> Dcache
    // shift address into cache fields
    // only fetch block aligned address
    addr = (addr >> (BYO_LEN + BLO_LEN)) << (BYO_LEN+BLO_LEN);
    printf("addr of cache:                       %x\n",addr);
    indx = (addr << TAG_LEN)>>(TAG_LEN+BYO_LEN+BLO_LEN);
    if(ID){
        //Icache
        Icache[indx].blk[blkNum].tag =addr >> (IDX_LEN+BYO_LEN+BLO_LEN);
        Icache[indx].blk[blkNum].valid = 1;
        Icache[indx].blk[blkNum].dirty = 0;
        for( i=0; i<BLK_SIZE; i++){
            Icache[indx].blk[blkNum].data[i] = iMem[addr+i];
        }
        #if mem2cachePrint
        printf("indx of cache:                       %x\n",indx);
        printf("Icache[indx].blk[blkNum].tag:        %x\n",Icache[indx].blk[blkNum].tag);
        printf("Icache[indx].blk[blkNum].valid:      %x\n",Icache[indx].blk[blkNum].valid);
        printf("Icache[indx].blk[blkNum].dirty:      %x\n",Icache[indx].blk[blkNum].dirty);
        for( i=0; i<BLK_SIZE; i++){
            printf("Icache[indx].blk[blkNum].data[%d]:    %x\n",i,Icache[indx].blk[blkNum].data[i]);
        }
        #endif
    }else{
        // Dcache
        Dcache[indx].blk[blkNum].tag =addr >> (IDX_LEN+BYO_LEN+BLO_LEN);
        Dcache[indx].blk[blkNum].valid = 1;
        Dcache[indx].blk[blkNum].dirty = 0;
        for( i=0; i<BLK_SIZE; i++){
            Dcache[indx].blk[blkNum].data[i] = iMem[addr+i];
        }
        #if mem2cachePrint
        printf("indx of cache:                       %x\n",indx);
        printf("Dcache[indx].blk[blkNum].tag:        %x\n",Dcache[indx].blk[blkNum].tag);
        printf("Dcache[indx].blk[blkNum].valid:      %x\n",Dcache[indx].blk[blkNum].valid);
        printf("Dcache[indx].blk[blkNum].dirty:      %x\n",Dcache[indx].blk[blkNum].dirty);
        for( i=0; i<BLK_SIZE; i++){
            printf("Dcache[indx].blk[blkNum].data[%d]:    %x\n",i,Dcache[indx].blk[blkNum].data[i]);
        }
        #endif
    }
}

#define cache2memPrint 1
void cache2mem(){
    // NOTE: only used for Dcache since Icache is never dirty
}

void pipe2mem(uint32_t addr, uint32_t word ){
    // ID dictates I cache or D cache
    // ID = 0 --> Dcache
    if(CACHE_ON){
        //NOTE: add write back shizznitz
        if(WR_BUFFER){
        }
    }else{
        // write word back to memory 
        dMem[addr] = word;
    }

}

#define mem2pipePrint 1
void mem2pipe(uint32_t addr, int ID){
    // ID dictates I cache or D cache
    // ID = 0 --> Dcache
    if(ID){
        if(CACHE_ON){
            // when cache is enabled
            // check cache for data 
            // if miss bring data from main memory to cache
            // write data to sMW->RD1
            uint32_t indx = (addr << TAG_LEN)>>(TAG_LEN+BYO_LEN+BLO_LEN);
            uint32_t blockOff = (addr << TAG_LEN+IDX_LEN)>>(TAG_LEN+IDX_LEN+BYO_LEN);
            int ret = isHit(addr,1);

            #if mem2pipePrint
            printf("index = %x\n",indx);
            printf("blockOff = %x\n",blockOff);
            #endif

            if(ret==-1){
                // miss
                // TODO: figure out how to decied which block to write when more than 1
                mem2cache(addr,1,0);
                sFD->MI = Icache[indx].blk[0].data[blockOff];
            }else{
                // hit
                sFD->MI = Icache[indx].blk[ret].data[blockOff];
            }
        }else{
            // cache is not enabled
            // grab instruction from memory
            sFD->MI = iMem[addr];
        }
    }else{
        // Dcache
        if(CACHE_ON){
            // when cache is enabled
            // check cache for data 
            // if miss bring data from main memory to cache
            // write data to sMW->RD1
            // TODO: add ID to isHit and mem2cache
            uint32_t indx = (addr << TAG_LEN)>>(TAG_LEN+BYO_LEN+BLO_LEN);
            uint32_t blockOff = (addr << TAG_LEN+IDX_LEN)>>(TAG_LEN+IDX_LEN+BYO_LEN);
            int ret = isHit(addr,0);

            #if mem2pipePrint
            printf("index = %x\n",indx);
            printf("blockOff = %x\n",blockOff);
            #endif

            if(ret==-1){
                // miss
                // TODO: figure out how to decied which block to write when more than 1
                mem2cache(addr,0,0);
                sMW->RD1 = Dcache[indx].blk[0].data[blockOff];
            }else{
                // hit
                sMW->RD1 = Dcache[indx].blk[ret].data[blockOff];
            }
        }else{
            // cache is not enabled
            // grab instruction from memory
            sMW->RD1 = dMem[addr];
        }
    }
}
