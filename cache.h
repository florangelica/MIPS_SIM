#ifndef CACHE_H_
#define CACHE_H_

#include<stdint.h>

#define CACHE_ON 0

#define I_CS 2          //sets per Icache
#define I_BLK_SIZE 16      //words per block
#define I_SET_ASS 1       //blocks per set
#define I_TAG_LEN 27      //bits of tag
#define I_VAL_LEN 1       //bits of valid
#define I_IDX_LEN 1       //bits of index
#define I_BLO_LEN 4      //bits of block offset
#define I_BYO_LEN 0       //bits of byte offset

#define D_CS 8          //sets per Dcache
#define D_BLK_SIZE 4      //words per block
#define D_SET_ASS 1       //blocks per set
#define D_TAG_LEN 27      //bits of tag
#define D_VAL_LEN 1       //bits of valid
#define D_IDX_LEN 3       //bits of index
#define D_BLO_LEN 2       //bits of block offset
#define D_BYO_LEN 0       //bits of byte offset

#define WR_BACK 0

#define D 0
#define I 1

struct I_BLOCK{
  uint32_t tag;
  uint32_t dirty;
  uint32_t valid;
  uint32_t blkAddr;
  int data[I_BLK_SIZE];
};
struct D_BLOCK{
  uint32_t tag;
  uint32_t dirty;
  uint32_t valid;
  uint32_t blkAddr;
  int data[D_BLK_SIZE];
};

struct I_SET{
  struct I_BLOCK blk[I_SET_ASS];
  uint32_t nxtBlk;
};
struct D_SET{
  struct D_BLOCK blk[D_SET_ASS];
  uint32_t nxtBlk;
};

struct I_SET *Icache;
struct D_SET *Dcache;
struct D_BLOCK WR_buf;

// memory functions
void initCache();
void freeCache();

//utility functions
uint32_t getTag(int ID,uint32_t addr);
uint32_t getIndx(int ID,uint32_t addr);
uint32_t getBlkOff(int ID,uint32_t addr);
uint32_t getBlkAddr(int ID,uint32_t addr);
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
