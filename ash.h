#ifndef ASH_H_INCLUDED
#define ASH_H_INCLUDED

#include "pcb.h"
#include "hash.h"

int insertBlocked(int*, pcb_t*); //14
pcb_t* removeBlocked(int*); //15
pcb_t* outBlocked(pcb_t*); //16
pcb_t* headBlocked(int*); //17
void initASH(); //18

#endif