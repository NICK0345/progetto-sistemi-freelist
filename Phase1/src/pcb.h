#ifndef PCB_H_INCLUDED
#define PCB_H_INCLUDED

#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
#include "list.h"

void initPcbs(); //1
void freePcb(pcb_t*); //2
pcb_t* allocPcb(); //3
void mkEmptyProcQ(struct list_head*); //4
int emptyProcQ(struct list_head*); //5
void insertProcQ(struct list_head*, pcb_t*); //6
pcb_t* headProcQ(struct list_head*); //7
pcb_t* removeProcQ(struct list_head*); //8
pcb_t* outProcQ(struct list_head*, pcb_t*); //9

int emptyChild(pcb_t*);
void insertChild(pcb_t*, pcb_t*);
pcb_t* removeChild(pcb_t*);
pcb_t* outChild(pcb_t*);

#endif