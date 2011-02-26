#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "instruction_sheet.h"
#include "instruction.h"
#include "interface.h"
#include "pp2.h"
#include "parser.h"

unsigned int get_instruction_code(instruction inst);
void get_pattern(instruction inst, unsigned int *pattern);
unsigned int load_program (instruction_sheet is, unsigned int current_it);

unsigned int get_instruction_code(instruction inst){
    unsigned int result = 0;
    
    assert(inst != NULL);
    
    switch(instruction_get_type(inst)){
		case LOOP_INST_CODE:{ result = LAZO_INST_CODE;
				break;}
		case ACQUIRE_INST_CODE:{result = 0x00;
								/*En este caso no interesa la instruccion ya /
								 * que se debe disparar el conversor AD*/
				break;}
		case PULSE_INST_CODE:{result = CONTINUE_INST_CODE;
				break;}
		case DELAY_INST_CODE:{result = CONTINUE_INST_CODE;
				break;}
		case ENDLOOP_INST_CODE:{result = RETL_INST_CODE;
				break;}
		case END_INST_CODE:{result = FIN_INST_CODE;
				break;}
	}
    return  result;
}

void get_pattern(instruction inst, unsigned int *pattern){
    assert(inst != NULL);
    
}

unsigned int load_program (instruction_sheet is, unsigned int current_it){
    
    unsigned int ins_count = 0, i = 0;
    instruction inst = NULL;
    int loop_level = -1; /*Al encontrar el primer loop se incrementa en 1 y 
                           quedamos en el nivel 0*/
    unsigned int data;
    unsigned int pattern;
    unsigned int inst_code = 0;
    unsigned int duration = 0;
    unsigned int result = 0;
    
    if(pp2_full_reset()){
		return 1;
	}else if(pp2_charge_mode_enabled()){
		return 1;
    }
    
    ins_count = instruction_sheet_instruction_count(is);
    
    for(i = 0; i < ins_count; i++){
        inst = instruction_sheet_get_nth_instruction(is, i);
        data = instruction_get_data(inst);
        get_pattern(inst, &pattern);
        duration = instruction_get_duration(inst);
        inst_code = get_instruction_code(inst);
        if(inst_code == LOOP_INST_CODE){/*Es una instruccion LOOP*/
            loop_level++;
        }else if(inst_code == ENDLOOP_INST_CODE){ /*Es una instruccion ENDLOOP*/
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
    return result;
}


int main ( int argc, char *argv[]){

    instruction_sheet inst_sheet = NULL;
    int current_it = 0, times; 
    unsigned int result = 0;   

    if(argc != 3){
        printf("Error. Uso: ./rmnPulseGenerator \
               <path del programa de pulsos>\t \
               <path de la hoja de delays>\n");
        return 0;
    }
    
    /****************PARSEAR HOJA DE PULSOS****************/
    inst_sheet = parse(argv[1]);
    if(inst_sheet == NULL){
        printf("Error:(mostrar mensaje de error devuelto) codigo: %i\n",result);
        return 0;
    }
    
    /*****************CARGAR DELAYS y DEMAS DATOS**********/
/*    generate_configuration_sheet(inst_sheet, "config");*/

    set_delay_values(inst_sheet, "config_conf");
    
    /*****************CARGAR LA RAM FASE*******************/
    instruction_sheet_print(inst_sheet);
    
    /*****************CARGAR EL PROGRAMA*******************/
    times = instruction_sheet_get_times(inst_sheet);
    while(current_it < times && result == 0){
        result = load_program (inst_sheet, current_it);
        if(result == 0){
            result = pp2_microprocessor_mode_enabled();
            if(result == 0){
                result = pp2_launch_pulse_sequence();
            }
        }
        /*¿como sabemos cuando termina de correr el programa*/
        current_it++;        
    }
    
    return result;        
}


#define RAM_SPACE_SIZE 16
bool load_phases_ram(instruction_sheet inst_sheet){
        unsigned int count_phases = 0,
                     count_phases_value = 0,
                     n = 0,
                     m = 0,
                     phase_value = 0,
                     next_base_address = 0;             
        phase p = NULL;
        bool result = true;
        
        assert(inst_sheet != NULL);
        
        count_phases = instruction_sheet_phase_count(inst_sheet);
        if (count_phases > RAM_SPACE_SIZE) result = false;
        else{
            next_base_address = 0;
            for(n = 0; n < count_phases; n++){
                p = instruction_sheet_get_nth_phase(inst_sheet, n);
                assert(p != NULL);
                
                set_base_address(p, next_base_address);
                
                count_phases_value = phase_count_values(p);
                for(m = 0; m < count_phases_value; m++){
                    phase_value = phase_nth_value(p, n);
                    
                    next_base_address++;    
                }
                



                
            
            
            }
        }


}













