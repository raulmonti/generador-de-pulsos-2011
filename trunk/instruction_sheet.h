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

#endif

