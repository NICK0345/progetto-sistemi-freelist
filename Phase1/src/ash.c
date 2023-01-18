#include "ash.h"
#include <stdio.h>

static semd_t semd_table[MAXPROC];
static struct list_head semdFree_h;
static DEFINE_HASHTABLE(semd_h, const_ilog2(MAXPROC*2));
/* semaphore descriptor (SEMD) data structure */

/*s_link e' la entry della tabella hash, s_key e' la chiave della tabella hash, semd_h e' il nome della tabella hash*/

//loop with hash_for_each_possible_ to get the semd with key value semAdd
semd_t* getSemd(int *semAdd){
    semd_t* semd;
    addokbuf("matching key = ");
    print_int(semAdd);
    addokbuf("\n");
    hash_for_each_possible(semd_h, semd, s_link, semAdd){
        print_int(semd->s_key);
        addokbuf("\n");
        if(semd->s_key == semAdd){
            addokbuf("found!\n");
            return semd;
        }
    }
    addokbuf("found nothing!\n");
    return NULL;
}

void initASH(){
    //inizializza la semdFree_h
    INIT_LIST_HEAD(&semdFree_h);
    for(int i = 0; i < MAXPROC; i++)
        list_add_tail(&semd_table[i].s_freelink, &semdFree_h);  
}

int insertBlocked(int *semAdd, pcb_t *p){
    /*
        1. Cercare in semd_h il semd con chiave semAdd;
        2. Se non esiste, prendere un semd dalla semdFree_h e inizializzarlo;
        3. Inserire il processo p nella coda dei processi bloccati del semd trovato (o creato);
        4. Restituire TRUE se al passo 2 non si e' riusciti ad inizializzare un semd, FALSE altrimenti.
    */
    semd_t* semd = getSemd(semAdd);
    if(semd == NULL){
        if(list_empty(&semdFree_h))
            return TRUE;
        //semd = primo elemento si semdFree_h
        semd = container_of(semdFree_h.next, semd_t, s_freelink);
        list_del(semdFree_h.next);
        semd->s_key = semAdd;
        INIT_LIST_HEAD(&semd->s_procq);
        hlist_add_head(&semd->s_link, semd_h);
    }
    hash_add(semd_h, &semd->s_link, semAdd);
    return FALSE;
}
pcb_t* removeBlocked(int *semAdd){
    /*
        1. Cercare in semd_h il semd con chiave semAdd;
        2. Se non esiste, restituire NULL;
        3. Altrimenti, rimuove il primo processo dalla coda dei processi bloccati del semd trovato;
        4. Se la coda dei processi bloccati del semd e' diventata vuota, rimuovere il semd dalla semd_h e inserirlo nella semdFree_h;
    */
    semd_t* semd = getSemd(semAdd);
    if(semd == NULL)
        return NULL;
    pcb_t* p = container_of(semd->s_procq.next, pcb_t, p_list);
    list_del(semd->s_procq.next);
    semd->s_key = NULL;
    if(list_empty(&semd->s_procq)){
        hlist_del_init(&semd->s_link);
        hash_del(&semd->s_link);
        list_add_tail(&semd->s_freelink, &semdFree_h);
    }
    return p;
}