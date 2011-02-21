/* Este tipo representa una fase en el programa. 
 * Las fases pueden tomar diferentes valores 
 * correspondientes a los introducidos con la 
 * función 'phase_add'. (se extraen de manera circular).
 */

#include "phase.h"
#include <stdio.h>
#include <stdlib.h>
#include "f_glist.h"
#include <assert.h>

#define DEBUG_ON


struct t_phase {
    unsigned int id;
    f_glist values;
};



phase phase_create(unsigned int id){
 
    phase result = NULL;
    
    result = calloc(1,sizeof(struct t_phase));
    if (result != NULL){
        result->id = id;
        result->values = f_glist_create();
    }
#ifdef DEBUG__ON    
    else{
        printf("Error, no se pudo reservar memoria para la phase %i.\n", id);
    }
#endif

    return result;
}



phase phase_destroy(phase p){

    assert(p != NULL);
    p->values = f_glist_destroy(p->values);
    free(p);
    p = NULL;
    
    return p;
}



int phase_id (phase p){
    assert(p!=NULL);
    return p->id;
}



/* phase_add: agrega un valor de phase 'degree' a la 
 * lista de valores posibles para la fase p
 */
void phase_add_value(phase p, float degrees){
    
    assert(p != NULL);
    f_glist_add (p->values, degrees);

}



/* phase_next_value: devuelve el siguiente valor de 
 * entre los asignados a la fase p
 */
float phase_next_value(phase p){

    assert(p!=NULL);
    return f_glist_next(p->values); 
}

void phase_print (phase p, int times){
    
    assert(p!=NULL);
    printf("Phase%u:\n", p->id);
    f_glist_print(p->values, times);

}

