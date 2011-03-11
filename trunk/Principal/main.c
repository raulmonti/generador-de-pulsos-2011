#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "instruction_sheet.h"
#include "instruction.h"
#include "interface.h"
#include "pp2.h"
#include "dds.h"
#include "parser.h"
#include <math.h>
#include "ad.h"

int main ( int argc, char *argv[]){
    instruction_sheet inst_sheet = NULL;
    int times = 0; 
    unsigned int result = 0, i = 0, kpc = 32, mps = 1000000;
    ad_t ad = ad_create();   

   /* if(argc != 3){
        printf("Error. Uso: ./rmnPulseGenerator \
               <path del programa de pulsos>\t \
               <path de la hoja de delays>\n");
        return 0;
    }*/
    /****************PARSEAR HOJA DE PULSOS****************//*LISTOOOOOO*/
    printf("ETAPA DE PARSEO DE HOJA DE PULSOS\n\n");
    inst_sheet = parse(argv[1]);
    if(inst_sheet == NULL){
        printf("Error:(mostrar mensaje de error devuelto) codigo: %i\n",result);
        return 1;
    }
    /****************CONFIGURACION DEL DDS****************//*LISTOOOOOO*/
    printf("ETAPA DE CONFIGURACION DEL DDS\n\n");
    dds_config();
    
    /****************CONFIGURACION DEL A/D****************//*LISTOOOOOO*/
    printf("ETAPA DE CONFIGURACION DEL A/D\n\n");
    ad_config(ad, mps, kpc, AD_MODO_CONTINUO);


    /****************CARGA DE FRECUENCIAS DEL DDS****************//*LISTOOOOOO*/
    printf("ETAPA DE CARGA DE FRECUENCIAS DEL DDS\n\n");
    dds_set_freq(5000000, 5000000); /* frec1 y frec2 deseadas = 5MHz */
    dds_enable();
    
    /*****************CARGAR DELAYS y DEMAS DATOS**********//*LISTOOOOOO*/
    printf("\nETAPA DE CARGA DE DELAYS Y CONVERSION DE LOOPS\n\n");

    //printf("\n>>> A continuacion se imprime la lista de instrucciones previo a \
    //        la conversion de loops y carga de delays y demas: \n\n\n");

   // instruction_sheet_print(inst_sheet);    

    instruction_sheet_config_instructions(inst_sheet, "config_conf");

    //printf("\n>>> A continuacion se imprime la lista de instrucciones final \
    //       previa a la carga del pp2: \n\n\n");

    //instruction_sheet_print(inst_sheet);

    /*****************CARGAR LA RAM FASE*******************/
    printf("ETAPA DE CARGA DE FASES\n\n");
    dds_load_phases_ram(inst_sheet, 0);

    /*****************CARGAR EL PROGRAMA*******************/
    printf("\nETAPA DE CARGA DE PROGRAMA DE PULSOS\n\n");
    result = pp2_load_program (inst_sheet);
    if(result == 0){
        result = pp2_microprocessor_mode_enabled();
        if(result == 0){
            result = pp2_launch_pulse_sequence();
        }
    }else{
          printf("Error en la carga del programa\n");      
    }
    
    result = ad_adquirir(ad);
    ad_to_file(ad);

    /* 
     *  Corro las repeticiones del experimento corriendo 
     *  la fase si asi se pidio.
     */

    /*times = instruction_sheet_get_times(inst_sheet);
   */
    times = -100;

    for(i = 1; i < abs(times); i++){
        if(times < 0){      /*times negativo indica cambio de fase en c/prueba*/
            dds_load_phases_ram(inst_sheet, i);
        }
        pp2_microprocessor_mode_enabled();
        result = pp2_launch_pulse_sequence();
        result = ad_adquirir(ad);
        ad_to_file(ad);
    }

    printf("\n>>>  TERMINANDO EL PROGRAMA CON RESULT = %i  <<<\n",result);   
    return 3;        
}
