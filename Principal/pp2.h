#ifndef _PP2_H_
#define _PP2_H_     

#include "instruction_sheet.h"

/*Direcciones de los registros de trabajo*/

#define COMMAND_REG_ADDR 0x50
#define STATUS_REG_ADDR 0x50
#define INSTRUCTION_REG_ADDR 0x51
#define TRANSFER_REG_ADDR 0x52

/*Patrones de configuracion*/
#define FULL_RESET 0x02
#define CHARGE_MODE 0x03
#define MICRO_MODE 0x00

/*Codigos de instruccion*/
#define CONTINUE_PP2_CODE 0x01
#define LAZO_PP2_CODE 0x02
#define RETL_PP2_CODE 0x03
#define FIN_PP2_CODE 0x07

/*Numero de bytes de las instrucciones aceptadas por el pp2 */
#define INST_LENGTH 8 
/*Numero de bucles anidados (comenzando desde 0)aceptadas por el pp2 */
#define MAX_LOOP_LEVEL 3

#define NO_ERROR_CODE 0
#define BUSY_ERROR_CODE 1
#define WRONG_INST_ERROR_CODE 2

 
/****Escribe en el registro de instruccion la instruccion correspondiente inst 
	*y en caso de necesitar argumento lo toma de pattern. Los valores posibles 
	*de inst son: 1 = CONTINUE
	* 			  2 = LAZO
	* 			  3 = RETL
	* 			  7 = FIN
    *Requiere: Haber ejecutado el metodo de configuracion correspondiente 
    * 		   para poder escribir en el dispositivo
    *Modifica: Nada
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado
    * 						   2 = Codigo de instruccion no valido 
*****/

unsigned int pp2_write_instruction( unsigned int pattern, unsigned int data, 
                                    int loop_level, unsigned int delay, 
                                    unsigned int inst_code);

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

/****Dispara la secuencia de pulsos previamente cargada
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado
*****/
unsigned int pp2_launch_pulse_sequence(void);

unsigned int pp2_load_program (instruction_sheet is);

#endif
