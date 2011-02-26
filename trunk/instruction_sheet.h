#ifndef INSTRUCTION_SHEET_H
#define INSTRUCTION_SHEET_H

#include "instruction.h"
#include "phase.h"

typedef struct inst_sheet_s *instruction_sheet;

/****
    *Requiere:Nada
    *Modifica: Nada
    *Retorna: Una nueva instacia de hoja de instrucciones
*****/
instruction_sheet instruction_sheet_create(void);

/****
    *Requiere: Nada
    *Modifica: Elimina por completo una hoja entera de instrucciones
    *Retorna: NULL
*****/

instruction_sheet instruction_sheet_destroy(instruction_sheet inst_sheet);

/****
    *Requiere: La fase ph debe ser no NULL
    *Modifica: Se aumenta la longitud de la lista en uno y se agrega ph al final de la lista de fases
    *Retorna: Una hoja de instrucciones con una fase mas
*****/

void instruction_sheet_add_phase(instruction_sheet inst_sheet, phase ph);

/****
    *Requiere: La instruccion inst debe ser no NULL
    *Modifica: Se aumenta la longitud de la lista en uno y se agrega inst al final de la lista de instrucciones
    *Retorna: Una hoja de instrucciones con una instruccion mas
*****/

void instruction_sheet_add_instruction(instruction_sheet inst_sheet, instruction inst);

/****Imprime por pantalla toda la estructura de la hoja de instrucciones inst_sheet
    *Requiere: Nada
    *Modifica: Nada
    *Retorna: Nada
*****/

void instruction_sheet_print(instruction_sheet inst_sheet);

/****
    *Requiere: inst_sheet != NULL
    *Modifica: Nada
    *Retorna: Un dato de tipo instruction que esta alojado en la posicion 
    * 		  n-esima de inst_sheet comenzando desde las posicion 0.
    * 		  En caso de un error que no viole la precondicion devuelve NULL
*****/
instruction instruction_sheet_get_nth_instruction(instruction_sheet inst_sheet, unsigned int n);

/****
    *Requiere: inst_sheet != NULL
    *Modifica: Nada
    *Retorna: La cantidad de intrucciones almacenadas en isnt_sheet
*****/

phase instruction_sheet_get_nth_phase(instruction_sheet inst_sheet, unsigned int n);


unsigned int instruction_sheet_instruction_count(instruction_sheet inst_sheet);

/****
    *Requiere: inst_sheet != NULL
    *Modifica: Nada
    *Retorna: La cantidad de intrucciones almacenadas en isnt_sheet
*****/
unsigned int instruction_sheet_phase_count(instruction_sheet inst_sheet);


/****
    *Requiere: inst_sheet != NULL
    *Modifica: el campo times (cantidad de repeticiones)
    *Retorna: nada
*****/
void instruction_sheet_set_times(instruction_sheet inst_sheet, unsigned int times);


/****
    *Requiere: inst_sheet != NULL
    *Modifica: nada
    *Retorna: el campo times (cantidad de repeticiones)
*****/
unsigned int instruction_sheet_get_times(instruction_sheet inst_sheet);

/****
    *Requiere: inst_sheet != NULL
    *Modifica: nada
    *Retorna: el campo path (ruta de la hoja de pulsos)
*****/
char *instruction_sheet_get_pulse_sheet_path(instruction_sheet inst_sheet);

/****
    *Requiere: inst_sheet != NULL y path != NULL
    *Modifica: Setea el valor del campo path de la hoja de pulsos
    *Retorna: Nada
*****/
void instruction_sheet_set_pulse_sheet_path(instruction_sheet inst_sheet, char* path); /*BORRAR PRONTO*/

/****
    *Requiere: inst_sheet != NULL
    *Modifica: Setea el valor del campo duration a todas las instrucciones 
    *          de tipo type e id igual a id
    *Retorna: Nada
*****/
void instruction_sheet_set_delay(instruction_sheet inst_sheet, unsigned int type, unsigned int id, unsigned int delay);
#endif

