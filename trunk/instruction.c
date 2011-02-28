#include "instruction.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


struct instruction_s{
    unsigned int id;
    unsigned int type;
    unsigned int data;
    unsigned int duration;
    unsigned int phase_shift;
};

instruction instruction_create(unsigned int id, int t, unsigned int p){

	instruction inst = NULL;

	inst = calloc(1, sizeof(struct instruction_s));
    inst->id = id;
    inst-> type = t;
    inst-> data = p;
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
    assert(inst != NULL);
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
        case LOOP_INST_CODE:{  printf("            type: LOOP\n");
                    break;}
        case ACQUIRE_INST_CODE:{  printf("            type: ACQUIERE\n");
                    break;}
        case PULSE_INST_CODE:{  printf("            type: PULSE\n");
                    break;}
        case DELAY_INST_CODE:{  printf("            type: DELAY\n");
                    break;}
        case ENDLOOP_INST_CODE:{  printf("            type: ENDLOOP\n");
                    break;}
        case END_INST_CODE:{  printf("            type: END\n");
                    break;} 
        default:{   printf("Error valor de type no valido\n");
                }
    }
    
    printf("            parameter: %u\n", inst->data);
    printf("            duration: %u\n", inst->duration);
    printf("\n");    
}


unsigned int instruction_get_data(instruction inst){
	assert(inst != NULL);
	return inst->data;
}

unsigned int instruction_get_type(instruction inst){
	assert(inst != NULL);
	return inst->type;
}

unsigned int instruction_get_id(instruction inst){
	assert(inst != NULL);
	return inst->id;
}


/*Indica que hay que agregar un corrimiento de valor 'shift'
sobre la lista de fases para esta instruccion*/
void instruction_phase_add_shift(instruction inst, int shift){

    assert(inst != NULL);
    inst->phase_shift += shift;
}

int instruction_phase_get_shift(instruction inst){
    assert(inst != NULL);
    return inst->phase_shift;
}

