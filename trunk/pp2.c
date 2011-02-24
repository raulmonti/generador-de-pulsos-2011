#include "pp2.h"
#include <stdlib.h>
#include <assert.h>
#include "lpt_linux.h"
/*#include "lpt_windows.h"*/

/*Direcciones de los registros de trabajo*/
#define COMMAND_REG_ADDR 0x50
#define STATUS_REG_ADDR 0x50
#define INSTRUCTION_REG_ADDR 0x51
#define TRANSFER_REG_ADDR 0x52

/*Patrones de configuracion*/
#define FULL_RESET 0x02
#define CHARGE_MODE 0x03
#define MICRO_MODE 0x00
#define LAUNCH_PULSE_SEQ 0x80

/*Numero de bytes de las instrucciones aceptadas por el pp2 */
#define INST_LENGTH 8 

/****Arma una instruccion de la forma:
	* 
	*|pattern | data | loop_level | inst_code | delay|
	* 
    *Requiere: Haber reservado memoria para todos los parametros
    *Modifica: Nada
    *Retorna: La instruccion 
    * 		   codigo de error: 0 = ok
    * 		  				    1 = Error dispositivo ocupado
    * 						    2 = Codigo de instruccion no valido 
*****/
unsigned int build_instruction (unsigned char *instruction, 
                                unsigned char *pattern, unsigned char *data, 
                                unsigned char loop_level, unsigned char *delay, 
                                unsigned char inst_code);
                                
/****Escribe en el registro de instruccion la instruccion 
	*correspondiente a inst armada de la siguiente forma:
	* 
	*|pattern | data | loop_level | inst_code | delay|
	* 
	*Los valores posibles de inst son: 0x01 = CONTINUE_INST_CODE
	* 			  					   0x02 = LAZO_INST_CODE
	* 			  					   0x03 = RETL_INST_CODE
	* 			  					   0x07 = FIN_INST_CODE
    *Requiere: Haber ejecutado el metodo de configuracion correspondiente 
    * 		   para poder escribir en el dispositivo
    *Modifica: El pedido de memoria para todos los parametros queda a 
    * 		   cargo del usuario
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado
    * 						   2 = Codigo de instruccion no valido 
*****/
unsigned int pp2_write_instruction(unsigned char *pattern, unsigned char *data, 
                                   unsigned char loop_level, unsigned char *delay, 
                                   unsigned char inst_code);

/****Resetea todos los registros del trabajo del dispositivo
    *Requiere: Nada
    *Modifica: Pone a todos los registros de trabajo con su valor por defecto
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado 
*****/
unsigned int pp2_full_reset(void);

/****Habilita el modo de carga en el dispositivo
    *Requiere: Nada
    *Modifica: El registro de comando del dispositivo
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado
*****/
unsigned int pp2_charge_mode_enabled(void);

/****Carga la RAM del dispositivo con una instruccion previamente cargada en el 
	*registro de instruccion
    *Requiere: Nada
    *Modifica: Carga la RAM del dispositivo con la instruccion previamente cargada
    * 		   en el registro de instruccion
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado
*****/
unsigned int pp2_transfer_instruction(void);

/****Habilita en modo microprocesador del dispositivo
    *Requiere: Nada
    *Modifica: El estado del registro de comando del dispositivo
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado 
*****/
unsigned int pp2_microprocessor_mode_enabled(void);

                                
/******************************************************************************/                                
unsigned int build_instruction (unsigned char *instruction, 
                                unsigned char *pattern, unsigned char *data, 
                                unsigned char loop_level, unsigned char *delay, 
                                unsigned char inst_code){
    unsigned char dataAux = 0;
    unsigned int result = 0;
    
    assert(instruction != NULL && pattern != NULL && data != NULL 
			&& delay != NULL);
    
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
        result = 2; /* Codigo de instruccion no valido */
    }
    
    return result;
}


unsigned int pp2_write_instruction(unsigned char *pattern, unsigned char *data, 
                                   unsigned char loop_level, unsigned char *delay, 
                                   unsigned char inst_code){
    
    int i = 0;
    unsigned int result = 0;
    unsigned char instruction[INST_LENGTH] = {0,0,0,0,0,0,0,0}; 
   
	assert(instruction != NULL && pattern != NULL && data != NULL 
			&& delay != NULL);
   
    if(!lpt_is_busy(LPT1_ADDR)){
        result = build_instruction(instruction, pattern, data, loop_level, 
                                   delay, inst_code);
        if(result == 0){                               
            for (i=INST_LENGTH-1; i>=0; i--){
                result = lpt_send_byte(INSTRUCTION_REG_ADDR, instruction[i]);
                if(result != 0) break;
            }
        }
    }
	return result;
}


unsigned int pp2_full_reset(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(COMMAND_REG_ADDR, FULL_RESET);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}

unsigned int pp2_charge_mode_enabled(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(COMMAND_REG_ADDR, CHARGE_MODE);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}

unsigned int pp2_transfer_instruction(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(TRANSFER_REG_ADDR, 0x00);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}


unsigned int pp2_microprocessor_mode_enabled(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(COMMAND_REG_ADDR, MICRO_MODE);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}

unsigned int pp2_launch_pulse_sequense(void){
	unsigned int result = 0;
	if(!lpt_is_busy(LPT1_ADDR)){
		result = lpt_send_byte(COMMAND_REG_ADDR, LAUNCH_PULSE_SEQ);
	}else{
		result = 1; /*Puerto ocupado*/
	}
	return result;
}



