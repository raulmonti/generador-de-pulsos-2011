#include "instruction_sheet.h"
#include "instruction.h"
#include <glib-2.0/glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct inst_sheet_s {
    GList* instruction_list;
    GList* phase_list;
};

instruction_sheet instruction_sheet_create(void){
    instruction_sheet inst_sheet = calloc(1, sizeof(struct inst_sheet_s));
    inst_sheet->instruction_list = NULL;
    inst_sheet->phase_list = NULL;

    return inst_sheet;

}

instruction_sheet instruction_sheet_destroy(instruction_sheet inst_sheet){

    int i = 0;
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

    unsigned int i = 1;

	if(inst_sheet != NULL){

		printf("Instruction Sheet\n");

        while(i<g_list_length(inst_sheet->instruction_list)){
            instruction_print(g_list_nth_data(inst_sheet->instruction_list, i));
            i++;
        }

        i = 1;

        while(i<g_list_length(inst_sheet->phase_list)){
            phase_print(g_list_nth_data(inst_sheet->phase_list, i), 1);
            i++;
        }
    }

}
