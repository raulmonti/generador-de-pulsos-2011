#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bstring/bstrlib.h"
#include "instruction_sheet.h"

#include "lexer.h"
#define PHASES "phases:"
#define PROGRAM "program:"
#define NEWLINE "\n"
#define ASSIGN "="

#define PHASE "ph"
#define PULSE "p"
#define DELAY "d"

#define LPARENT "("
#define RPARENT ")"

bstring get_line(Lexer *l);
bool consume_spaces(Lexer *l);
bool line_begin_with (bstring line, const char* pattern);

bool is_program_line(bstring line);
bool is_phase_line(bstring line);

bool phase_header(bstring line);
bool program_header(bstring line);

bool parse_phase(bstring line, instruction_sheet sheet);
bool parse_pulse(bstring line, instruction_sheet sheet);
bool parse_delay_line(bstring line, instruction_sheet sheet);




int main(void){
  FILE *file = NULL;
  Lexer *l = NULL;
  bstring item = NULL;
  bool parse_ok = true;
  
  instruction_sheet sheet = NULL;
  
  file = fopen("sintaxis_ejemplo", "r");
  if (file == NULL){
    printf("El archivo no existe\n");
    return 1;
  }
  
  l = lexer_new(file);
  
  
  /* Declaraciones nuevas */
  sheet = instruction_sheet_create();
  
  
  
  
  
  parse_ok = false;
  
  item = get_line(l);
  if (item == NULL) printf("Archivo vacío\n");
  else parse_ok = phase_header(item);
  
  
  
  
  
  
  
  
  
  if (parse_ok){
      item = get_line(l);
      while(item != NULL && is_phase_line(item) && parse_ok){
            printf("Linea leida: %s\n", item->data);
            parse_ok = parse_phase(item, sheet);
            bdestroy(item);
            item = get_line(l);
    }
  }else{
    printf("No se encuentra 'phases:'\n");
  }
  
  if(item != NULL) parse_ok = program_header(item);
  if (parse_ok){
    item = get_line(l);
    while(item != NULL && is_program_line(item) && parse_ok){
        printf("Linea leida: %s\n", item->data);
        if (line_begin_with (item, PULSE)){
            parse_ok = parse_pulse(item, sheet);
        }
        
        if (line_begin_with (item, DELAY)){
            parse_ok = parse_delay_line(item, sheet);
        }
        
        bdestroy(item);
        item = get_line(l);
    }
    
    if (item != NULL) printf("Error en el bloque 'program'\n");
      
  }else{
    printf("No se encuentra 'program:'\n");
  }
  
  printf("\n\n\n");
  instruction_sheet_print(sheet);
  
  printf("****************instruction_sheet_get_nth_instruction()*************************\n");
  instruction_print(instruction_sheet_get_nth_instruction(sheet, 0));
  
  printf("Cantidad de instrucciones: %u\n", instruction_sheet_instruction_count(sheet));
  
  printf("Cantidad de fases: %u\n", instruction_sheet_phase_count(sheet));
  
  
  return 0;
}


bool consume_spaces(Lexer *l){
#define SPACE " "
#define TAB "\t"
#define WHITESPACES SPACE TAB
    assert(l != NULL);
    assert(!lexer_is_off(l));
    
    lexer_skip(l, WHITESPACES);
    
    return !lexer_is_off(l);
}


bstring get_line(Lexer *l){
    bstring item = NULL;
    bool parse_ok = true;
    
    assert(l != NULL);
    assert(!lexer_is_off(l));

    lexer_skip(l, BLANK);
    if (!lexer_is_off(l)) parse_ok = true;
    else parse_ok = false;
    
    if (parse_ok){
        lexer_next_to(l, NEWLINE);
        if (!lexer_is_off(l)){
            item = lexer_item(l);
            bltrimws(item);
            brtrimws(item);
            if (blength(item) == 0){
                bdestroy(item);
                item = NULL;
            }
        }
    }
    
    return item;
}

bool line_begin_with(bstring line, const char* pattern){
    
    bstring dummy = NULL;
    bool result = true;

    assert (line != NULL);
    assert (pattern != NULL);
    
    dummy = bfromcstr(pattern);
    assert(dummy != NULL);
    
    result = binstr(line, 0, dummy) != BSTR_ERR;
    bdestroy(dummy);
    
    return result;

}

bool is_program_line(bstring line){
    
    bool result = true;
    
    /* PRE: */
    assert(line != NULL);
    
    result =  line_begin_with(line, PULSE) ||
              line_begin_with(line, DELAY);
    
    return result;
}

bool is_phase_line(bstring line){

    bool result = true;
    
    /* PRE: */
    assert(line != NULL);
    
    result =  line_begin_with(line, PHASE);
    
    return result;
}

bool phase_header(bstring line){
    assert (line != NULL);
    
    return biseqcstr(line, PHASES);
}



bool program_header(bstring line){
    assert (line != NULL);
    
    return biseqcstr(line, PROGRAM);
}

bool parse_phase(bstring line, instruction_sheet sheet){
    
    bstring item = NULL;
    
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    
    bool parse_ok = true;
    phase ph = NULL;
    
    assert(line != NULL);
    assert(line_begin_with(line, PHASE));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    /* PRE: */
    lexer_next(l, PHASE);
    item = lexer_item(l);
    assert(biseqcstr(item, PHASE));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    parse_ok = !lexer_is_off(l);
    
    /* phase phase_create(unsigned int id); */
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else{
            unsigned int phase_id = 0;
            phase_id = atoi((const char*) item->data);
            ph = phase_create(phase_id);
        }
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        parse_ok = consume_spaces(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        lexer_next(l, ASSIGN);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        parse_ok = biseqcstr(item, ASSIGN);
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        parse_ok = consume_spaces(l);
    }

    if (parse_ok){
        do{
            /* -- INVARIANTE: -- */
            /* La cinta està abierta */
            assert(!lexer_is_off(l)); 
            /* El próximo caracter a leer es distinto de BLANK */
            lexer_next(l, BLANK);
            item = lexer_item(l);
            assert(blength(item) == 0);
            bdestroy(item);
            
            lexer_next(l, DIGIT);
            parse_ok = !lexer_is_off(l);
            if (parse_ok){
                assert(!lexer_is_off(l));
                item = lexer_item(l);
                if (blength(item) == 0) parse_ok = false;
                else{
                    unsigned int phase_value = 0;
                    
                    phase_value = atoi((const char*)item->data);
                    phase_add_value(ph, (float) phase_value);
                
                }
                bdestroy(item);
            }

            consume_spaces(l);
        }while(!lexer_is_off(l) && parse_ok);
    
    }
    
    if (parse_ok)
        instruction_sheet_add_phase(sheet, ph);

    return parse_ok;
}

bool parse_pulse(bstring line, instruction_sheet sheet){

    bstring item = NULL;
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    bool parse_ok = true;
    
    instruction instr = NULL;
    
   
    assert(line != NULL);
    assert(line_begin_with(line, PULSE));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    /* PRE: */
    lexer_next(l, PULSE);
    item = lexer_item(l);
    assert(biseqcstr(item, PULSE));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    parse_ok = !lexer_is_off(l);
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else{
            unsigned int pulse_id = 0;
            instruction_type pulse_type = 2;
            
            pulse_id = atoi((const char*) item->data);
            instr = instruction_create(pulse_id, pulse_type, 0);
        }
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        lexer_next(l, LPARENT);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));        
        item = lexer_item(l);
        parse_ok = biseqcstr(item, LPARENT);
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));    
        lexer_next(l, PHASE);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));        
        item = lexer_item(l);
        parse_ok = biseqcstr(item, PHASE);
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        lexer_next(l, DIGIT);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        /*else printf("phase_pulse_value: %s\n", item->data);*/
        bdestroy(item);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        lexer_next(l, RPARENT);
        parse_ok = !lexer_is_off(l);
    }
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        parse_ok = biseqcstr(item, RPARENT);
        bdestroy(item);
    }
    
    if (parse_ok)
        instruction_sheet_add_instruction(sheet, instr);
    
    lexer_destroy(l);
    fclose(fstream);
    
    return parse_ok;

}


bool parse_delay_line(bstring line, instruction_sheet sheet){

    bstring item = NULL;
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    
    bool parse_ok = true;
    instruction instr = NULL;
    
    assert(line != NULL);
    assert(line_begin_with(line, DELAY));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    /* PRE: */
    lexer_next(l, DELAY);
    item = lexer_item(l);
    assert(biseqcstr(item, DELAY));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    parse_ok = !lexer_is_off(l);
    
    if (parse_ok){
        assert(!lexer_is_off(l));
        item = lexer_item(l);
        if (blength(item) == 0) parse_ok = false;
        else{ 
            /* instruction instruction_create(unsigned int id, instruction_type t, unsigned int p); */   
            unsigned int delay_id = 0;
            instruction_type pulse_type = 3;
            
            delay_id = atoi((const char*) item->data);
            instr = instruction_create(delay_id, pulse_type, 0);
        }
        bdestroy(item);
    }

    if (parse_ok)
        instruction_sheet_add_instruction(sheet, instr);
    
    lexer_destroy(l);
    fclose(fstream);
       
    return parse_ok;

}

