#ifndef CACHE_H_
#define CACHE_H_

#include<stdint.h>

#define BLK_SIZE 1
#define I_CS 8
#define D_CS 8
#define TAG_LEN 27
#define VAL_LEN 1
#define IDX_LEN 3
#define BO_LEN 2

struct CACHE{
  uint32_t tag: TAG_LEN;
  uint32_t valid: VAL_LEN;
  uint32_t index: IDX_LEN;
  uint32_t bo: BO_LEN; 
  uint32_t block[BLK_SIZE];
};

struct CACHE *Icache;
struct CACHE *Dcache;

void initCache();
void freeCache();

#endif
