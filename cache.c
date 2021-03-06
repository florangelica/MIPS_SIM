#include<stdlib.h>
#include<stdio.h>
#include "cache.h"
#include "structs.h"
#include "memory.h"

// Memory functions
void initCache(){
    Icache = (struct I_SET *) malloc(I_CS * sizeof(struct I_SET));
    Dcache = (struct D_SET *) malloc(D_CS * sizeof(struct D_SET));
}
void freeCache(){
    free(Icache);
    free(Dcache);
}

// utility functions
uint32_t getTag(int ID, uint32_t addr){
    if(ID){
        return addr >> (I_IDX_LEN+I_BYO_LEN+I_BLO_LEN);
    }else{
        return addr >> (D_IDX_LEN+D_BYO_LEN+D_BLO_LEN);
    }
        
}
uint32_t getIndx(int ID, uint32_t addr){
    if(ID){
        return (addr << I_TAG_LEN)>>(I_TAG_LEN+I_BYO_LEN+I_BLO_LEN);
    }else{
        return (addr << D_TAG_LEN)>>(D_TAG_LEN+D_BYO_LEN+D_BLO_LEN);
    }
}
uint32_t getBlkOff(int ID,uint32_t addr){
    if(ID){
        return (addr << I_TAG_LEN+I_IDX_LEN)>>(I_TAG_LEN+I_IDX_LEN+I_BYO_LEN);
    }else{
        return (addr << D_TAG_LEN+D_IDX_LEN)>>(D_TAG_LEN+D_IDX_LEN+D_BYO_LEN);
    }
}
uint32_t getBlkAddr(int ID,uint32_t addr){
    if(ID){
        return (addr >> (I_BYO_LEN + I_BLO_LEN)) << (I_BYO_LEN+I_BLO_LEN);
    }else{
        return (addr >> (D_BYO_LEN + D_BLO_LEN)) << (D_BYO_LEN+D_BLO_LEN);
    }
}

void nxtInSet(int ID, uint32_t addr){
    uint32_t indx = getIndx(ID,addr);
    // increment nxtBlk
    if(ID){
        // Icache
        Icache[indx].nxtBlk = Icache[indx].nxtBlk + 1;
        if(Icache[indx].nxtBlk >= I_SET_ASS){
            //reset to zero
            Icache[indx].nxtBlk = 0;
        }
    }else{
        // Dcache
        Dcache[indx].nxtBlk = Dcache[indx].nxtBlk + 1;
        if(Dcache[indx].nxtBlk >= D_SET_ASS){
            //reset to zero
            Dcache[indx].nxtBlk = 0;
        }
    }
}

// main functions
#define isHitPrint 0
int isHit(int ID, uint32_t addr){
    // hit return block number
    // miss return -1
    // ID == 0 --> Dcache
    uint32_t indx, tag, blkNum;
    indx = getIndx(ID,addr);
    tag = getTag(ID,addr);

    if(ID){
        // go to index and compare tags
        for( blkNum=0; blkNum<I_SET_ASS; blkNum++){
            if((Icache[indx].blk[blkNum].tag == tag)&&(Icache[indx].blk[blkNum].valid )){
                #if isHitPrint
                printf("isHit -- Icache: hit!\n");
                #endif
                return blkNum;
            }else{
                #if isHitPrint
                printf("isHit -- Icache: miss\n");
                #endif
                return -1;
            }
        }
    }else{
        //Dcache
        // go to index and compare tags
        for( blkNum=0; blkNum<D_SET_ASS; blkNum++){
            if((Dcache[indx].blk[blkNum].tag == tag)&&(Dcache[indx].blk[blkNum].valid )){
                #if isHitPrint
                printf("isHit -- Dcache: hit!\n");
                #endif
                return blkNum;
            }else{
                #if isHitPrint
                printf("isHit -- Dcache: miss\n");
                #endif
                return -1;
            }
        }
    }
}



#define mem2cachePrint 1
void mem2cache(int ID, uint32_t addr, int blkNum){
    int i;
    uint32_t indx;
    // ID == 0 --> Dcache
    // shift address into cache fields
    // only fetch block aligned address
    //addr = getBlkAddr(addr);
//    printf("addr of cache:                       %x\n",addr);
    indx = getIndx(ID,addr);

    if(ID){
        //Icache
        Icache[indx].blk[blkNum].tag = getTag(ID,addr);
        Icache[indx].blk[blkNum].valid = 1;
        Icache[indx].blk[blkNum].dirty = 0;
        for( i=0; i<I_BLK_SIZE; i++){
            Icache[indx].blk[blkNum].data[i] = iMem[addr+i];
        }
        #if mem2cachePrint
        printf("m2c -- indx of cache:                       %x\n",indx);
        printf("m2c -- Icache[indx].blk[blkNum].tag:        %x\n",Icache[indx].blk[blkNum].tag);
        printf("m2c -- Icache[indx].blk[blkNum].valid:      %x\n",Icache[indx].blk[blkNum].valid);
        printf("m2c -- Icache[indx].blk[blkNum].dirty:      %x\n",Icache[indx].blk[blkNum].dirty);
        for( i=0; i<I_BLK_SIZE; i++){
            printf("m2c -- Icache[indx].blk[blkNum].data[%d]:    %x\n",i,Icache[indx].blk[blkNum].data[i]);
        }
        #endif
    }else{
        // Dcache
        Dcache[indx].blk[blkNum].tag = getTag(ID,addr);
        Dcache[indx].blk[blkNum].valid = 1;
        Dcache[indx].blk[blkNum].dirty = 0;
        Dcache[indx].blk[blkNum].blkAddr = addr;
        for( i=0; i<D_BLK_SIZE; i++){
            Dcache[indx].blk[blkNum].data[i] = iMem[addr+i];
        }
        #if mem2cachePrint
        printf("m2c -- indx of cache:                       %x\n",indx);
        printf("m2c -- Dcache[indx].blk[blkNum].tag:        %x\n",Dcache[indx].blk[blkNum].tag);
        printf("m2c -- Dcache[indx].blk[blkNum].valid:      %x\n",Dcache[indx].blk[blkNum].valid);
        printf("m2c -- Dcache[indx].blk[blkNum].dirty:      %x\n",Dcache[indx].blk[blkNum].dirty);
        for( i=0; i<D_BLK_SIZE; i++){
            printf("m2c -- Dcache[indx].blk[blkNum].data[%d]:    %x\n",i,Dcache[indx].blk[blkNum].data[i]);
        }
        #endif
    }
}



#define cache2memPrint 0
void cache2mem(){
    // move contents of WR_buf to memory
    int i;
    for( i=0; i<D_BLK_SIZE; i++){
        iMem[WR_buf.blkAddr + i] = WR_buf.data[i];
    }
}


// returns the location a blk should be stored in
uint32_t getBlock(int ID, uint32_t addr){
    int val = isHit(ID,addr);
    if(val == -1){
        uint32_t indx = getIndx(ID,addr);
        uint32_t blkNum;
        // miss
        // determine which blk of the set data should be moved into
        if(ID){
            // Icache
            blkNum = Icache[indx].nxtBlk;
            nxtInSet(ID,addr);
            mem2cache(ID,addr,blkNum);
            return blkNum;
        }else{
            // Dcache
            blkNum = Dcache[indx].nxtBlk;
            nxtInSet(ID,addr);
            // check if data in block is dirty
            if(Dcache[indx].blk[blkNum].dirty == 1){
                // move block to WR_buf
                int i;
                WR_buf.blkAddr = Dcache[indx].blk[blkNum].blkAddr;
                for(i=0; i <D_BLK_SIZE; i++){
                    WR_buf.data[i] = Dcache[indx].blk[blkNum].data[i];
                }
                //call move WR_buf to memory
                cache2mem();
            }
            mem2cache(ID, addr, blkNum);
            return blkNum;
        }
    }else{
        // hit
        return val;
    }
}


#define p2mPrint 0
void pipe2mem(uint32_t addr, uint32_t word ){
    if(CACHE_ON){
        int i;
        uint32_t blkOff = getBlkOff(D,addr);
        addr = getBlkAddr(D,addr);
        uint32_t indx = getIndx(D,addr);
        int blkNum = getBlock(D,addr);
        // write word into cache
        Dcache[indx].blk[blkNum].data[blkOff] = word;
        #if p2mPrint
        printf("indx of cache:                       %x\n",indx);
        printf("Dcache[indx].blk[blkNum].tag:        %x\n",Dcache[indx].blk[blkNum].tag);
        printf("Dcache[indx].blk[blkNum].valid:      %x\n",Dcache[indx].blk[blkNum].valid);
        printf("Dcache[indx].blk[blkNum].dirty:      %x\n",Dcache[indx].blk[blkNum].dirty);
        for( i=0; i<D_BLK_SIZE; i++){
            printf("Dcache[indx].blk[blkNum].data[%d]:    %x\n",i,Dcache[indx].blk[blkNum].data[i]);
        }
        #endif
        if(WR_BACK){
            // set block to dirty
            Dcache[indx].blk[blkNum].dirty =1;
            printf("writeback enabled\n");
        }else{
            // write through
            for(i=0; i<D_BLK_SIZE; i++){
                WR_buf.data[i] = Dcache[indx].blk[blkNum].data[i];
            }
            // push WR_buf to memory
            cache2mem();
        }
    }else{
        // write word back to memory 
        iMem[addr] = word;
    }
}



#define mem2pipePrint 1
void mem2pipe(int ID, uint32_t addr){
    // ID dictates I cache or D cache
    // ID = 0 --> Dcache
    uint32_t blkOff = getBlkOff(ID,addr);
    addr = getBlkAddr(ID,addr);
    uint32_t indx = getIndx(ID,addr);
    uint32_t blkNum;
    #if mem2pipePrint
    printf("m2p -- index = %x\n",indx);
    printf("m2p -- blkOff = %x\n",blkOff);
    #endif

    if(ID){
        if(CACHE_ON){
                blkNum = getBlock(ID,addr);
                sFD->MI = Icache[indx].blk[blkNum].data[blkOff];
        }else{
            // cache is not enabled
            // grab instruction from memory
            sFD->MI = iMem[addr];
        }
    }else{
        // Dcache
        if(CACHE_ON){
            blkNum = getBlock(ID,addr);
            sMW->RD = Dcache[indx].blk[blkNum].data[blkOff];
        }else{
            // cache is not enabled
            // grab instruction from memory
            sMW->RD = iMem[addr];
        }
    }
}

void flush( uint32_t addr){
    //get block aligned address 
    uint32_t indx = getIndx(D,addr);
    uint32_t blkNum,i;
    // Dcache
    for( blkNum=0; blkNum<D_SET_ASS; blkNum++){
        if(Dcache[indx].blk[blkNum].valid){
            printf("flush -- Dcache: valid data to flush\n");
            WR_buf.blkAddr = Dcache[indx].blk[blkNum].blkAddr;
            for(i=0; i<D_BLK_SIZE; i++){
                WR_buf.data[i] = Dcache[indx].blk[blkNum].data[i];
            }
            cache2mem();
        }
    }
}
