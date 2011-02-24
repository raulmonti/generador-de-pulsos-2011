#include "interface.h"

#include <stdlib.h>
#include "lexer.h"

#define INITIAL_LEN 30
#define INCREMENT   20

#define SUFFIX "_conf"

struct array_t{
	int* items;
	unsigned int len;
	unsigned int max_len;
 };

 typedef struct array_t *iarray;
 
 /* Funciones Auxiliares */
static iarray iarray_create();
static void iarray_add(iarray array, int value);
static bool iarray_checkvalue(iarray, int value);
static iarray iarray_destroy(iarray array);

static bool get_id_and_delay(bstring line); 
 

 /*
  Genera un archivo con nombre: sheet->filename ++ "_config",
  en el mismo path donde se encuentra 'sheet', con el siguiente
  formato:
  
  sheet: archivoprueba
  ----------------------------
  phases:
	ph1 = 1 2 3 4
	ph4 = 7 8
  program:
	d1
	p1(ph4)
	d2
	d1
	p1(ph5)
	p5(ph1)
  ----------------------------
  
  Generaría:
  configuration_sheet: archivoprueba_conf
  -----------------------------
  d1:
  p1:
  d2:
  p5:
  -----------------------------
  
 */
  void generate_configuration_sheet(instruction_sheet sheet){

	unsigned int len = 0,
	             i = 0,
				 line_len = 0;
	instruction aux = NULL;
	FILE *f = NULL;
	bstring filenameconf = NULL,
	        line = NULL;
	iarray delay_list = NULL,
	       pulse_list = NULL;
	
	/* PRE: */
	assert(sheet != NULL);
	
	len = instruction_sheet_instruction_count(sheet);
	assert(len != 0);
	
	filenameconf = bstrcpy(sheet->path);
	assert(filenameconf != NULL);
	
	bcatcstr(filenameconf, SUFFIX);
	
	f = fopen(filenameconf->data, "w");
	delay_list = iarray_create();
	pulse_list = iarray_create();
	
	for(i = 0; i < len; i++){
		line = bfromcstr("");
		aux = instruction_sheet_get_nth_instruction(sheet, i);
		assert(aux != NULL);
	
		if (get_instruction_type == PULSE){
			if (!iarray_checkvalue(pulse_list, instruction_id)){
				iarray_add(pulse_list, instruction_id);
				bcatcstr(line, "p");
				bcatcstr(line, instruction_id);
				bcatcstr(line, ":\n");
			}
			
	
		}
		
		if (get_instruction_type == DELAY){
			if (!iarray_checkvalue(delay_list, instruction_id)){
				iarray_add(delay_list, instruction_id);
				bcatcstr(line, "d");
				bcatcstr(line, instruction_id);
				bcatcstr(line, ":\n");
			}
		}
		
		line_len = blength(line);
		if (line_len != 0)
			fwrite((void*)line->data, 1, line_len, f);			
			
		bdestroy(line);
	}
	
	delay_list = iarray_destroy(delay_list);
	pulse_list = iarray_destroy(pulse_list);
	
 }
 
 
 bool set_delay_values(instruction_sheet sheet){
	FILE *f = NULL;
	bstring filenameconf = NULL,
	        item = NULL;
	Lexer *l = NULL;
	bool result = true;
	unsigned int instruction_id = 0;
	
	filenameconf = bstrcpy(sheet->path);
	assert(filenameconf != NULl);
	
	bcatcstr(filenameconf, SUFFIX);
	
	f = fopen(filenameconf->data, "r");
	if (f == NULL) result = false;
	
	if(!result) printf("No se ha encontrado el archivo\n");
	
	l = lexer_new(f);
	assert(l != NULL);
	
	/***********CODIGO AGREGADO SOLO PARA HACER COMPILAR**************/
	return true;
	
	
	
	
 
 }
 
 
static bool get_id_and_delay(Lexer *l, unsigned int* id, unsigned int* delay){
	bool result = true;
	
	bstring item = NULL;
	
	assert(l != NULL);
	assert(!lexer_is_off(l));
	
	lexer_next(l, LOWER);
	result = !lexer_is_off(l);
	
	if (result){
		item = lexer_item(l);
		result = biseqcstr(item, "p") || biseqcstr(item, "d");
		bdestroy(item);
	}
	
	if (result){
		lexer_next(l, DIGIT);
		result = !lexer_is_off(l);
	}
	
	if (result){
		item = lexer_item(l);
		if (blength(item) == 0) result = false;
		else *id = atoi(item->data);
		bdestroy(item);
	}
	
	if (result){
		lexer_next(l, ":");
		result = !lexer_is_off(l);
	}
	
	if (result){
		item = lexer_item(l);
		result = biseqcstr(item, ":");
		bdestroy(item);
	}
	
	if (result){
		lexer_next(l, DIGIT);
		result = !lexer_is_off(l);
	}
	
	if (result){
		item = lexer_item(l);
		if (blength(item) == 0) result = false;
		else *delay = atoi(item->data);
		bdestroy(item);
	}
	
	

	return result;

}















 
 
 
 static iarray iarray_create(){
	iarray result = NULL;
	
	iarray = (iarray)calloc(1, sizeof(struct array_t));
	assert(iarray != NULL);
	
	iarray->items = (int*) calloc(INITIAL_LEN, sizeof(int));
	assert(iarray->items != NULL);
	
	iarray->len = 0;
	iarray->max_len = INITIAL_LEN;
	
	return iarray;
 
 }
 
 static void iarray_add(iarray array, int value){
 
	assert(array != NULL);
	
	if (array->item == array->maxlen){
		array->max_len = array->max_len + INCREMENT;
		array->item = (int*) realloc(max_len, sizeof(int));
	}
	
	array->item[array->len] = value;
	array->len++;
	
}
 static bool iarray_checkvalue(iarray, int value){

	bool result = true;
	int i = 0;
	
	result = false;
	for (i = 0; i < iarray->len && !result; i++)
		if (iarray[i] == value) result = true;
	
	return result;

}
 static iarray iarray_destroy(iarray array){
	
	assert(array != NULL);
	
	free(array->items);
	free(array);
	
	return NULL;

 }
