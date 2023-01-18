#include "pcb.h"


static pcb_t pcbFree_table[MAXPROC];
static struct list_head pcbFree_h;

void initPcbs(){
    mkEmptyProcQ(&pcbFree_h);
    for(int i = 0; i < MAXPROC; i++){
        freePcb(&pcbFree_table[i]);
    }
}
void freePcb(pcb_t* p){
    list_add_tail(&p->p_list, &pcbFree_h);
}
pcb_t* allocPcb(){
    if(list_empty(&pcbFree_h)){
        return NULL;
    }
    pcb_t* p = container_of(pcbFree_h.next, pcb_t, p_list);
    INIT_LIST_HEAD(&p->p_child);
    list_del(pcbFree_h.next);
    return p;
}
void mkEmptyProcQ(struct list_head* head){
    INIT_LIST_HEAD(head);
}
int emptyProcQ(struct list_head* head){
    return list_empty(head);
}
void insertProcQ(struct list_head* head, pcb_t* p){
    struct list_head* curr;
    list_for_each(curr, head){
        pcb_t* curr_pcb = container_of(curr, pcb_t, p_list);
        if(curr_pcb->p_time > p->p_time){
            list_add_tail(&p->p_list, curr);
            return;
        }
    }
    list_add_tail(&p->p_list, head);
}
pcb_t* headProcQ(struct list_head* head){
    if(list_empty(head)){
        return NULL;
    }
    return container_of(head->next, pcb_t, p_list);
}
pcb_t* removeProcQ(struct list_head* head){
    if(list_empty(head)){
        return NULL;
    }
    pcb_t* p = container_of(head->next, pcb_t, p_list);
    list_del(head->next);
    return p;
}
pcb_t* outProcQ(struct list_head* head, pcb_t* p){
    struct list_head* curr;
    list_for_each(curr, head){
        pcb_t* curr_pcb = container_of(curr, pcb_t, p_list);
        if(curr_pcb == p){
            list_del(curr);
            return p;
        }
    }
    return NULL;
}

int emptyChild(pcb_t* p){
    return p!=NULL&&list_empty(&p->p_child);
}
void insertChild(pcb_t* prnt, pcb_t* p){
    list_add_tail(&p->p_sib, &prnt->p_child);
    p->p_parent = prnt;
}
pcb_t* removeChild(pcb_t* p){
    if(list_empty(&p->p_child)){
        return NULL;
    }
    pcb_t* child = container_of(p->p_child.next, pcb_t, p_sib);
    list_del(p->p_child.next);
    return child;
}
pcb_t* outChild(pcb_t* p){
    struct list_head* curr;
    pcb_t* parent = p->p_parent;
    if(parent == NULL){
        return NULL;
    }
    list_for_each(curr, &p->p_parent->p_child){
        pcb_t* curr_pcb = container_of(curr, pcb_t, p_sib);
        if(curr_pcb == p){
            list_del(curr);
            return p;
        }
    }
    return NULL;
}