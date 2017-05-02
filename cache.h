#ifndef CACHE_H_
#define CACHE_H_

#include<stdint.h>

#define CACHE_ON 1

#define true 1
#define false 0

#define BLK_SIZE 8      //words per block
#define SET_ASS 2       //blocks per set
#define I_CS 8          //sets per Icache
#define D_CS 8          //sets per Dcache
#define TAG_LEN 21      //bits of tag
#define VAL_LEN 1       //bits of valid
#define IDX_LEN 8       //bits of index
#define BLO_LEN 3       //bits of block offset
#define BYO_LEN 0       //bits of byte offset


struct BLOCK{
  uint32_t tag:            TAG_LEN;
  uint32_t dirty:          VAL_LEN;
  uint32_t valid:          VAL_LEN;
  uint32_t data[BLK_SIZE];
};

struct SET{
  struct BLOCK blk[SET_ASS];
};

struct SET *Icache;
struct SET *Dcache;

void initCache();
void freeCache();

int isHit(uint32_t addr, int ID);
void mem2cache(uint32_t addr,int ID ,int blkNum);
void cache2mem();
void pipe2mem();
void mem2pipe(uint32_t addr, int ID);

#endif
