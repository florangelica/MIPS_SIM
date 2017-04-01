#ifndef STAGES_H_
#define STAGES_H_

#include<stdint.h>
#include"structs.h"

void fetch();
void decode();
void execute();
void memory();
void writeBack();
void shadowShift();
void printPipe();
#endif
