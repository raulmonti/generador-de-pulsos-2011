#include "instruction.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct instruction_s{
    unsigned int id;
    instruction_type type;
    unsigned int parameter;
    unsigned int duration;
};


instruction instruction_create(unsigned int id, instruction_type t, unsigned int p){

	instruction inst = NULL;

	inst = calloc(1, sizeof(struct instruction_s));
    inst->id = id;
    inst-> type = t;
    inst-> parameter = p;
    inst-> duration = 0;

    return inst;
}


instruction instruction_destroy(instruction inst){
    
    assert(inst != NULL);
    
    if (inst != NULL){
        free(inst);
        inst = NULL;
    }
    
    return inst;
}


void instruction_set_duration(instruction inst, unsigned int d){
    assert(isnt != NULL);
    inst->duration = d;
}


unsigned int instruction_get_duration(instruction inst){
    assert(inst != NULL);
    return inst->duration;
}


void instruction_print(instruction inst){
    printf("Instruction:\n");    
    printf("            id: %u\n", inst->id);

    switch(inst->type){
        case 0:{  printf("            type: LOOP\n");
                    break;}
        case 1:{  printf("            type: ACQUIERE\n");
                    break;}
        case 2:{  printf("            type: PULSE\n");
                    break;}
        case 3:{  printf("            type: DELAY\n");
                    break;} 
        default:{   printf("Error valor de type no valido\n");
                }
    }
    
    printf("            parameter: %u\n", inst->parameter);
    printf("            duration: %u\n", inst->duration);
    printf("\n");    
}


