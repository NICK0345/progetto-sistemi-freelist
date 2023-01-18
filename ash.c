#include "ash.h"


static semd_t semd_table[MAXPROC];
static struct list_head semdFree_h;
static struct list_head semd_h;

int insertBlocked(int* key, pcb_t* p){
    semd_t* semd = getSemd(key);
    if(semd == NULL){
        semd = allocSemd();
        if(semd == NULL){
            return TRUE;
        }
        semd->s_key = key;
        insertSemd(semd);
    }
    insertProcQ(&semd->s_procq, p);
    return FALSE;
}
pcb_t* removeBlocked(int* key){
    semd_t* semd = getSemd(key);
    if(semd == NULL){
        return NULL;
    }
    pcb_t* p = removeProcQ(&semd->s_procq);
    if(emptyProcQ(&semd->s_procq)){
        removeSemd(semd);
        freeSemd(semd);
    }
    return p;
}
pcb_t* outBlocked(pcb_t* p){
    semd_t* semd = getSemd(p->p_semAdd);
    if(semd == NULL){
        return NULL;
    }
    pcb_t* out = outProcQ(&semd->s_procq, p);
    if(emptyProcQ(&semd->s_procq)){
        removeSemd(semd);
        freeSemd(semd);
    }
    return out;
}
pcb_t* headBlocked(int* key){
    semd_t* semd = getSemd(key);
    if(semd == NULL){
        return NULL;
    }
    return headProcQ(&semd->s_procq);
}
void initASH(){
    mkEmptyProcQ(&semdFree_h);
    mkEmptyProcQ(&semd_h);
    for(int i = 0; i < MAXPROC; i++){
        freeSemd(&semd_table[i]);
    }
}