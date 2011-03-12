#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "main_functions.h"
#include <string.h>

int main ( int argc, char *argv[]){

    int result = 0;

    if(argv[1] == NULL){
        printf("Faltan argumentos para correr el programa\n");
        return 1;
    }

    if(!strcmp(argv[1],"mainParsePulseProgram")){
        if(argv[2]==NULL || argv[3]==NULL || argv[4]==NULL){
            printf("Faltan argumetos para mainParsePulseProgram\n");
            return 1;
        }        
        main_parse_pulse_program (argv[2], argv[3], argv[4]);
    }else if(!strcmp(argv[1],"mainRun"){
        if(argv[1]==NULL || argv[2]==NULL || argv[3]==NULL || argv[4]==NULL ||
           argv[5]==NULL || argv[6]==NULL || argv[7]==NULL || argv[8]==NULL ||
           argv[9]==NULL ||){
            printf("Faltan argumetos para mainRun\n");
            return 1;
        }
        result = main_run (char *pulse_sheet, char *delay_sheet, unsigned int mps, 
                       unsigned int kpc, unsigned int modo, unsigned int frec1,
                       unsigned int frec2, char *ad_output, int times )
    }else{
        return 2;
    } 

    printf("\n>>>  TERMINANDO EL PROGRAMA CON RESULT = %i  <<<\n",result);   
    return 0;        
}
