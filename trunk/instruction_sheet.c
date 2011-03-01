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




unsigned int instruction_sheet_remove_instructions
		(instruction_sheet is,unsigned int from, unsigned int to){
	
	GList *inst_list = NULL;
	unsigned int i = 0;
	unsigned int result = 0;
		
	assert(is != NULL);
	assert(from < to && to < instruction_sheet_instruction_count(is));	
	
	for(i = from; i <= to; i++){
			inst_list = g_list_nth(is->instruction_list, from);
			is->instruction_list = g_list_remove_link(is->instruction_list, inst_list);
			printf("BORRADO>>>\n");
			instruction_print(inst_list->data);
			inst_list->data = instruction_destroy(inst_list->data);
			g_list_free(inst_list);
	} 
	
	return result;	
}

unsigned int instruction_sheet_insert_instructions
		(instruction_sheet is, GList* instructions, unsigned int from){
	
	unsigned int i = 0, length = 0;
	instruction pointer = NULL;
	length = g_list_length(instructions);
	
	for(i = 0; i < length; i++){
	    pointer = g_list_nth_data(instructions, i);
	    is->instruction_list = g_list_insert(is->instruction_list, pointer, from+i);	
	}
	
	return 0;
}



phase instruction_sheet_get_phase(instruction_sheet inst_sheet, unsigned int id){

    int i = 0, cota = 0;
    phase result = NULL, p = NULL;
    
    assert(inst_sheet != NULL);
    cota = g_list_length(inst_sheet->phase_list);
    
    
    for(i = 0; i < cota; i++){
        p = g_list_nth_data(inst_sheet->phase_list, i);
        if(phase_id(p) == id) 
            result = p;
    }
    
    return result;
}


