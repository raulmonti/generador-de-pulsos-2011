#include "instruction_sheet.h"
#include "instruction.h"
#include <glib-2.0/glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct inst_sheet_s {
    GList* instruction_list;
    GList* phase_list;
    unsigned int times; /*Numero de veces que se realiza el experimento*/
    char *pulse_sheet_path;
};

instruction_sheet instruction_sheet_create(void){
    instruction_sheet inst_sheet = calloc(1, sizeof(struct inst_sheet_s));
    inst_sheet->instruction_list = NULL;
    inst_sheet->phase_list = NULL;
    inst_sheet->times = 0;

    return inst_sheet;

}

instruction_sheet instruction_sheet_destroy(instruction_sheet inst_sheet){

    unsigned int i = 0;
    assert(inst_sheet != NULL);

    if (inst_sheet != NULL){
    
        for(i = 0; i < g_list_length(inst_sheet->instruction_list); i++){
            instruction_destroy(g_list_nth_data(inst_sheet->instruction_list,i));
        }
    
        g_list_free(inst_sheet->instruction_list);

        for(i = 0; i < g_list_length(inst_sheet->phase_list); i++){
            printf("%i",i);
            phase_destroy(g_list_nth_data(inst_sheet->phase_list,i));
        }
        
        g_list_free(inst_sheet->phase_list);

        free(inst_sheet);
        inst_sheet = NULL;
    }

    return inst_sheet;
    
}

void instruction_sheet_add_phase(instruction_sheet inst_sheet, phase ph){
    assert(inst_sheet != NULL);
    
    inst_sheet->phase_list = g_list_append(inst_sheet->phase_list, ph);
}

void instruction_sheet_add_instruction(instruction_sheet inst_sheet, instruction inst){
    assert(inst_sheet != NULL); 

    inst_sheet->instruction_list = g_list_append(inst_sheet->instruction_list, inst);
}

void instruction_sheet_print(instruction_sheet inst_sheet){

    unsigned int i = 0;

	if(inst_sheet != NULL){

		printf("Instruction Sheet\n");

        while(i<g_list_length(inst_sheet->instruction_list)){
            instruction_print(g_list_nth_data(inst_sheet->instruction_list, i));
            i++;
        }

        i = 0;

        while(i<g_list_length(inst_sheet->phase_list)){
            phase_print(g_list_nth_data(inst_sheet->phase_list, i), 1);
            i++;
        }
    }
}

instruction instruction_sheet_get_nth_instruction(instruction_sheet inst_sheet, unsigned int n){
	instruction result = NULL;
	
	assert(inst_sheet != NULL);
	
	result = g_list_nth_data(inst_sheet->instruction_list, n);
	
	return result;
}

phase instruction_sheet_get_nth_phase(instruction_sheet inst_sheet, unsigned int n){
	phase result = NULL;
	
	assert(inst_sheet != NULL);
	
	result = g_list_nth_data(inst_sheet->phase_list, n);
	
	return result;
}


unsigned int instruction_sheet_instruction_count(instruction_sheet inst_sheet){
	unsigned int count = 0;
	
	assert(inst_sheet != NULL);
	
	count = g_list_length(inst_sheet->instruction_list);
    
	return count;
}

unsigned int instruction_sheet_phase_count(instruction_sheet inst_sheet){
	unsigned int count = 0;
	
	assert(inst_sheet != NULL);
	
	count = g_list_length(inst_sheet->phase_list);
	
	return count;
}


void instruction_sheet_set_times(instruction_sheet inst_sheet, unsigned int times){

    assert(inst_sheet != NULL);
    inst_sheet->times = times;

}



unsigned int instruction_sheet_get_times(instruction_sheet inst_sheet){

    assert(inst_sheet != NULL);
    return inst_sheet->times;

}

char *instruction_sheet_get_pulse_sheet_path(instruction_sheet inst_sheet){

    assert(inst_sheet != NULL);
    return inst_sheet->pulse_sheet_path;
}

void instruction_sheet_set_pulse_sheet_path(instruction_sheet inst_sheet, char* path){
	assert(inst_sheet != NULL);
	assert(path != NULL);
	
    inst_sheet->pulse_sheet_path = path;
}

void instruction_sheet_set_delay(instruction_sheet inst_sheet, unsigned int type, unsigned int id, unsigned int delay){
    unsigned int i = 0;
    unsigned int count = 0;
    instruction inst = NULL;
    
    assert(inst_sheet != NULL);
    
    count = instruction_sheet_instruction_count(inst_sheet);
    
    for(i=0; i < count; i++){
        inst = instruction_sheet_get_nth_instruction(inst_sheet, i);
        if((instruction_get_type(inst) == type) && (instruction_get_id(inst) == id)){
            instruction_set_duration(inst, delay);
        }
    }
}
