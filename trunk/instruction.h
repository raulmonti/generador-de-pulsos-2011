#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define LOOP_INST_CODE 0
#define ACQUIRE_INST_CODE 1
#define PULSE_INST_CODE 2
#define DELAY_INST_CODE 3
#define ENDLOOP_INST_CODE 4
#define END_INST_CODE 5

typedef struct instruction_s *instruction;

/****
	*Requiere: t=LOOP o t=ACQUIERE o t=DELAY o t=PULSE
	*Modifica: Nada
	*Retorna: Una nueva instancia de una instruccion con identificador id, de tipo
	*           type, parametro p y duracion 0
*****/
instruction instruction_create(unsigned int id, int t, unsigned int p);

/****
	*Requiere: Nada
	*Modifica: Elimina por completo la instruccion inst
	*Retorna: NULL
*****/

instruction instruction_destroy(instruction inst);

/****
	*Requiere: inst != NULL
	*Modifica: El valor de la duracion de inst a d
	*Retorna: Nada
*****/

void instruction_set_duration(instruction inst, unsigned int d);


/****
	*Requiere: inst != NULL
	*Modifica: Nada
	*Retorna: El valor de la duracion de inst
*****/

unsigned int instruction_get_duration(instruction inst);

/****
	* Imprime toda la instruccino inst por pantalla
	*Requiere: Nada
	*Modifica: Nada
	*Retorna: Nada
*****/
void instruction_print(instruction inst);

/****
	*Requiere: inst != NULL
	*Modifica: Nada
	*Retorna: El valor del campo data de la instruccion inst
*****/
unsigned int instruction_get_data(instruction inst);

/****
	*Requiere: inst != NULL
	*Modifica: Nada
	*Retorna: El valor del codigo del campo type de la instruccion inst
*****/
unsigned int instruction_get_type(instruction inst);

/****
	*Requiere: inst != NULL
	*Modifica: Nada
	*Retorna: El valor el id de la instruccion inst
*****/
unsigned int instruction_get_id(instruction inst);


void instruction_phase_add_shift(instruction inst, int shift);


int instruction_phase_get_shift(instruction inst);

#endif

