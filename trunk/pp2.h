#ifndef _PP2_H_
#define _PP2_H_   

/*Codigos de instruccion*/
#define CONTINUE_INST_CODE 0x01
#define LAZO_INST_CODE 0x02
#define RETL_INST_CODE 0x03
#define FIN_INST_CODE 0x07

/****Configura y realiza todos los pasos para cargar una instruccion en 
	*el dispositivo
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado
    * 						   2 = Codigo de instruccion no valido 
*****/
unsigned int pp2_load_instruction(unsigned char *pattern, unsigned char *data, 
                                  unsigned char loop_level, unsigned char *delay, 
                                  unsigned char inst_code);

/****Dispara la secuencia de pulsos previamente cargada
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: codigo de error: 0 = ok
    * 		  				   1 = Error dispositivo ocupado
*****/
unsigned int pp2_launch_pulse_sequense(void);

#endif
