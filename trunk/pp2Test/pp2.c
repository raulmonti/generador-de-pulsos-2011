#include "pp2.h"
#include "lpt_linux.h"
/*#include "lpt_windows.h"*/





unsigned int pp2_write_instruction(unsigned char *pattern, unsigned char *data, 
                                   unsigned char loop_level, unsigned char *delay, 
                                   unsigned char inst_code){
    
    int i = 0;
    unsigned int result = 0;
    unsigned char instruction[INST_LENGTH] = {0,0,0,0,0,0,0,0}; 
   
    if(!lpt_is_busy(LPT1_ADDR)){
        result = build_instruction(instruction, pattern, data, loop_level, 
                                   delay, inst_code);
        if(!result){                               
            for (i=INST_LENGTH-1; i>=0; i--){
                result = lpt_send_byte(LPT1_ADDR, instruction[i]);
                if(result != 0) break;
            }
        }
    }
	return result;
}




unsigned int pp2_full_reset(void){
	return 0;
}

unsigned int pp2_charge_mode_enabled(void){
	return 0;
}

unsigned int pp2_transfer_instruction(void){
	return 0;
}

unsigned int pp2_microprocessor_mode_enabled(void){
	return 0;
}

unsigned int pp2_launch_pulse_sequense(void){
	return 0;
}

unsigned int build_instruction (unsigned char *instruction, 
                                unsigned char *pattern, unsigned char *data, 
                                unsigned char loop_level, unsigned char *delay, 
                                unsigned char inst_code){
    unsigned char dataAux = 0;
    unsigned int result = 0;
    
    result = (inst_code == CONTINUE_INST_CODE || inst_code == LAZO_INST_CODE ||
              inst_code == RETL_INST_CODE || inst_code == FIN_INST_CODE );

    if(result){
        
        loop_level = loop_level << 3;
        dataAux = data[1];
        data[1] = data[1] << 5;
        dataAux = dataAux >> 3;
        data[0] = data[0] << 5;
        data[0] = data[0] | dataAux;
        
        instruction[7] = delay[3];
        instruction[6] = delay[2];
        instruction[5] = delay[1];
        instruction[4] = delay[0];
        instruction[3] = (data[1]|loop_level)|inst_code;
        instruction[2] = data[0];
        instruction[1] = pattern[1];
        instruction[0] = pattern[0];
    }else{
        result = 4; /* Codigo de instruccion no valido */
    }
    
    return result;
}

