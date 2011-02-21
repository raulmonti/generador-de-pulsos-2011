#define _GNU_SOURCE

#include <stdio.h>
#include <assert.h>

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

bool parse_phase(bstring line);
bool parse_pulse(bstring line);
bool parse_delay_line(bstring line);




int main(void){
  FILE *file = NULL;
  Lexer *l = NULL;
  bstring item = NULL;
  bool parse_ok = true;
  
  file = fopen("sintaxis_ejemplo", "r");
  if (file == NULL){
    printf("El archivo no existe\n");
    return 1;
  }
  
  l = lexer_new(file);
  
  item = get_line(l);
  
  parse_ok = phase_header(item);
  if (parse_ok){
      item = get_line(l);
      while(item != NULL && is_phase_line(item)){
            printf("Linea leida: %s\n", item->data);
            parse_phase(item);
            item = get_line(l);
    }
  }else{
    printf("No se encuentra 'phases:'\n");
  }
  
  
  parse_ok = program_header(item);
  if (parse_ok){
    item = get_line(l);
    while(item != NULL && is_program_line(item) && parse_ok){
        printf("Linea leida: %s\n", item->data);
        if (line_begin_with (item, PULSE)){
            parse_ok = parse_pulse(item);
        }
        
        if (line_begin_with (item, DELAY)){
            parse_ok = parse_delay_line(item);
        }
        
        item = get_line(l);
    }
    
    if (item != NULL) printf("Error en el bloque 'program'\n");
      
  }else{
    printf("No se encuentra 'program:'\n");
  }
  
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

    lexer_next_to(l, LOWER);
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
    
    assert(line != NULL);
    
    result =  line_begin_with(line, PULSE) ||
              line_begin_with(line, DELAY);
    
    return result;
}

bool is_phase_line(bstring line){

    bool result = true;
    
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

bool parse_phase(bstring line){
    
    bstring item = NULL;
    
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    
    bool parse_ok = true;
    
    assert(line != NULL);
    assert(line_begin_with(line, PHASE));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    lexer_next(l, PHASE);
    item = lexer_item(l);
    assert(biseqcstr(item, PHASE));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    if (lexer_is_off(l)) parse_ok = false;
    else{
        item = lexer_item(l);
        printf("phase_id: %s\n", item->data);
        bdestroy(item);
    }
    
    if (parse_ok)
        if (!consume_spaces(l)) parse_ok = false;
    
    
    if (parse_ok)
        lexer_next(l, ASSIGN);
        
    if (lexer_is_off(l)) parse_ok = false;
    else{
        item = lexer_item(l);
        bdestroy(item);
    }
    
    if (parse_ok)
        if (!consume_spaces(l)) parse_ok = false;
    
    if (parse_ok)
        lexer_next(l, DIGIT);
        
    if (lexer_is_off(l)) parse_ok = false;
    else{
        do{
            item = lexer_item(l);
            if (blength(item) == 0){
                    printf("Error en la declaraciòn de 'phases'\n");
                    break;
            }
            
            printf("\tphase_value: %s\n", item->data);
            bdestroy(item);
            
            
           if(consume_spaces(l))
                lexer_next(l, DIGIT);
            
        }while (!lexer_is_off(l));
    }
    
    return parse_ok;
}

bool parse_pulse(bstring line){

    bstring item = NULL;
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    bool parse_ok = true;
    
    assert(line != NULL);
    assert(line_begin_with(line, PULSE));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    lexer_next(l, PULSE);
    item = lexer_item(l);
    assert(biseqcstr(item, PULSE));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    if (lexer_is_off(l)) parse_ok = false;
    else{
        item = lexer_item(l);
        if (blength(item) > 0)
            printf("\tpulse_id: %s\n", item->data);
        else
            parse_ok = false;
    }
    
    if (parse_ok)
        lexer_next(l, LPARENT);
    
    if (lexer_is_off(l)) parse_ok = false;
    
    if (parse_ok){
        item = lexer_item(l);
        parse_ok = biseqcstr(item, LPARENT);
        bdestroy(item);
    }
    
    if (parse_ok)
        lexer_next(l, PHASE);
 
    if (lexer_is_off(l)) parse_ok = false;
    else{
        if (parse_ok){
            item = lexer_item(l);
            if(!biseqcstr(item, PHASE)) parse_ok = false;
        }
    }
    
    if (parse_ok)
        lexer_next(l, DIGIT);
    
    if (lexer_is_off(l)) parse_ok = false;
    else{
        if (parse_ok){
            item = lexer_item(l);
            printf("\tphase_pulse_value: %s\n", item->data);
        }
    }
    
    if (parse_ok)
        lexer_next(l, RPARENT);
    
    if (lexer_is_off(l)) parse_ok = false;
    
    if (parse_ok){
        item = lexer_item(l);
        parse_ok = biseqcstr(item, RPARENT);
        bdestroy(item);
    }


    return parse_ok;

}

bool parse_delay_line(bstring line){

    bstring item = NULL;
    int line_len = 0;
    FILE *fstream = NULL;
    Lexer *l = NULL;
    
    bool parse_ok = true;
    
    assert(line != NULL);
    assert(line_begin_with(line, DELAY));
    
    line_len = blength(line);
    
    fstream = fmemopen(line->data, line_len, "r");
    assert(fstream != NULL);
    
    l = lexer_new(fstream);
    assert(l != NULL);
    
    lexer_next(l, DELAY);
    item = lexer_item(l);
    assert(biseqcstr(item, DELAY));
    bdestroy(item);
    
    lexer_next(l, DIGIT);
    if (lexer_is_off(l)) parse_ok = false;
    else{
        item = lexer_item(l);
        printf("\tdelay_id: %s\n", item->data);
    }

    return parse_ok;

}

