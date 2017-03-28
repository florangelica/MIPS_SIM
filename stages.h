#ifndef STAGES_H_
#define STAGES_H_

#include<stdint.h>
#include"structs.h"

void fetch( uint32_t PC);
void decode();
void execute();
void memory();
void writeBack();
#endif
