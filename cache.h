#ifndef CACHE_H_
#define CACHE_H_

#include<stdint.h>

#define CACHE_ON 1

#define BLK_SIZE 4      //words per block
#define SET_ASS 1       //blocks per set
#define I_CS 8          //sets per Icache
#define D_CS 8          //sets per Dcache
#define TAG_LEN 25      //bits of tag
#define VAL_LEN 1       //bits of valid
#define IDX_LEN 3       //bits of index
#define BLO_LEN 2       //bits of block offset
#define BYO_LEN 2       //bits of byte offset

#define WR_BACK 0

struct BLOCK{
  uint32_t tag:            TAG_LEN;
  uint32_t dirty:          VAL_LEN;
  uint32_t valid:          VAL_LEN;
  uint32_t blkAddr;
  int data[BLK_SIZE];
};

struct SET{
  struct BLOCK blk[SET_ASS];
  uint32_t nxtBlk;
};

struct SET *Icache;
struct SET *Dcache;
struct BLOCK WR_buf;

// memory functions
void initCache();
void freeCache();

//utility functions
uint32_t getTag(uint32_t addr);
uint32_t getIndx(uint32_t addr);
uint32_t getBlkOff(uint32_t addr);
uint32_t getBlkAddr(uint32_t addr);
void nxtInSet(int ID, uint32_t addr);

// main functions
int isHit(int ID, uint32_t addr);
void mem2cache(int ID, uint32_t addr ,int blkNum);
void cache2mem();
uint32_t getBlock(int ID, uint32_t addr);
void pipe2mem(uint32_t addr, uint32_t word );
void mem2pipe(int ID, uint32_t addr);
void flush(uint32_t addr);
#endif
