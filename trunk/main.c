#include <stdio.h>
#include <stdlib.h>
#include "instruction_sheet.h"


unsigned char get_instruction_code(instruction inst){
    assert(inst != NULL);
    return  intruction_get_type(inst);
}

void get_data(instruction inst, unsigned char **data){
    assert(inst != NULL);
    
}

void get_pattern(instruction inst, unsigned char **pattern){
    assert(inst != NULL);
    
}

unsigned int load_program (instruction_sheet is, unsigned int current_it ){
    
    unsigned int ins_count = 0, i = 0;
    instruction inst = NULL;
    int loop_level = -1; /*Al encontrar el primer loop se incrementa en 1 y 
                           quedamos en el nivel 0*/
    unsigned char data[2] = {0,0};
    unsigned char pattern[2] = {0,0};
    unsigned char inst_code = 0;
    unsigned int duration = 0;
    
    if(pp2_full_reset()){
		return 1;
	}else if(pp2_charge_mode_enabled()){
		return 1;
    }
    
    ins_count = instruction_sheet_instruction_count(is);
    
    for(i = 0; i < ins_count; i++){
        inst = instruction_sheet_get_nth_instruction(is, i);
        get_data(inst, &data);
        get_pattern(inst, &pattern);
        duration = instruction_get_duration(inst);
        inst_code = get_instruction_code(inst);
        if(inst_code == 0){/*Es una instruccion LOOP*/
            loop_level++;
        }else if(inst_code == 4){ /*Es una instruccion ENDLOOP*/
            loop_level--;
        }
        result = pp2_write_instruction(pattern, data, loop_level, 
                                        duration, 
                                        inst_code);
        if(result == 1 || result == 2){ 
        /*1 = Puero ocupado , 2 = Codigo de instruccion no valido*/
			return result;
		}else if(pp2_transfer_instruction()){
			return 1;
		}
    }
}


int main ( int argc, char *argv[]){

    instruction_sheet inst_sheet = NULL;
    int current_it = 0, times;    

    if(argc != 3){
        printf("Error. Uso: ./rmnPulseGenerator \
               <path del programa de pulsos>\t \
               <path de la hoja de delays>\n");
        return 0;
    }
    
    /****************PARSEAR HOJA DE PULSOS****************/
    result = parse (&inst_sheet, argv[1]);
    if(result != 0){
        printf("Error:(mostrar mensaje de error devuelto) codigo: %i\n",result);
        return 0;
    }
    
    /*****************CARGAR DELAYS y DEMAS DATOS**********/
    generate_configuration_sheet(inst_sheet);

    set_delay_values(inst_sheet);
    
    /*****************CARGAR LA RAM FASE*******************/
    
    
    /*****************CARGAR EL PROGRAMA*******************/
    times = instruction_sheet_get_times(inst_sheet);
    while(current_it < times && result == 0){
        result = load_program (inst_sheet, current_it);
        if(result == 0){
            result = pp2_microprocessor_mode_enabled();
            if(result == 0){
                result = pp2_launch_pulse_sequense();
            }
        }
        /*¿como sabemos cuando termina de correr el programa*/
        current_it++;        
    }
    
    return result;        
}

