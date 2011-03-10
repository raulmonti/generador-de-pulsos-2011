#include "ad.h"
#include "lpt_windows.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
                   
bool ad_leer_buffers(unsigned int kpc, unsigned int adqm);

bool ad_reset_modo_pc (unsigned int kpc, unsigned int adqm);

void ad_to_file(short *canala, short *canalb, unsigned int kpc);    

/******************************************************************************/



bool ad_config (unsigned int mps, unsigned int kpc, unsigned int adqm){

    bool result = false;  
    unsigned char N = 0;
    unsigned char conf = 0x00;


    /* Modo PC para configurarlo */
    //direccion(0x0b);
    //escritura(0x00);
    
    /* Configuracion de la cantidad de muestras por segundo. */

    if(mps < 10000000 && mps > 39215){
        N = 10000000/mps;
        printf("Configurando cantidad de mps: %u\n", 255-N);
        getchar();
        result = direccion(0x0c);
        if(!result)
            result = escritura(255-N);
        if(!result)
            result = direccion(0x0d);
        if(!result)
            result = escritura(0x00);
    }else {
        printf("Error en la cantidad de muestras por segundo\n");        
        result = 1;
    }
        
    assert(kpc==1 || kpc==2 || kpc==4 || kpc==8 || kpc==16 || kpc==32 || kpc==64);
        
    /* modo continuo -> b1 = 1. Modo modulado -> b1 = 0 */
    if(adqm == AD_MODO_CONTINUO)
        conf = conf | 0x02;        
        
    /* Kpc = tamano del buffer de adquisicion en Kb: */    
    if(kpc==64){
            conf=conf|0x70;
    }else if(kpc==32){
            conf=conf|0x60;
    }else if(kpc==16){
            conf=conf|0x50;
    }else if(kpc==8){
            conf=conf|0x40;
    }else if(kpc==4){
            conf=conf|0x30;
    }else if(kpc==2){
            conf=conf|0x20;
    }else if(kpc==1){
            conf=conf|0x10;
    }else if(kpc != 0){
        result = 1;
    }

    



    if(!result)
        result = direccion(0x0b);   
    if(!result)
        result = escritura(conf|0x81); //Reset contador y modo AD bit b7 = 1  
    if(!result)
        result = escritura(conf|0x01); //bit b7 = 0

    return result;
}



/******************************************************************************/

unsigned int ad_adquirir(unsigned int kpc, unsigned int adqm){
    unsigned int result = 0;
    unsigned int b = 0;

    if(!result)          
        result = direccion(0x0b);
    if (!result)
        result = startLeer();    
    if (!result){
        do{
            b=leer();
            //printf("leyo b = %u\n",b);
        }while(!(b&0x01));
        result = endLeer();
        printf("leyo b = %u\n",b);
     }
                             
     /*OBTENGO EL RESULTADO*/
       
    if(!result)
        result = ad_leer_buffers(kpc, adqm);

    return result;
}



/******************************************************************************/


bool ad_leer_buffers(unsigned int kpc, unsigned int adqm){
    
    unsigned int i = 0, result = 0;
    char *canal_a = NULL, *canal_b = NULL, *canal_ab = NULL,  aux = 0;
    short *canala = NULL, *canalb = NULL;
    
    canal_a = calloc(1, sizeof(char)*kpc*1024);
    canal_b = calloc(1, sizeof(char)*kpc*1024);
    canal_ab = calloc(1, sizeof(char)*kpc*1024);    
    canala = calloc(1, sizeof(short)*kpc*1024);
    canalb = calloc(1, sizeof(short)*kpc*1024);  
  
    ad_reset_modo_pc(kpc, adqm);


    direccion(0x08);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_b[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
    
    ad_reset_modo_pc(kpc, adqm);
//mandar conf como estaba | 0x80
//mandar conf como estaba 
    
    direccion(0x09);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_ab[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
        
    ad_reset_modo_pc(kpc, adqm);
    
    direccion(0x0a);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(kpc*1024); i++)
        canal_a[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
   
    ad_reset_modo_pc(kpc, adqm);


/*NO ESTOY SEGURO SI HAY QUE HACER ESTO*/
 
   //direccion(0x0b);  /* direccionar el registro de comando */  
	//      escritura(0xf2);   /* reset contador de direcciones */

    /* Construyo el resultado final A ab B -> Aa bB*/

    for(i = 0; i<(kpc*1024);i++){
        canala[i] = canal_a[i];
        canala[i] = canala[i]<<4;
        aux = canal_ab[i]>>4;
        canala[i] = canala[i] | aux;
    }
    

    for(i = 0; i<(kpc*1024);i++){
        canalb[i] = canal_b[i];
        canalb[i] = canalb[i]<<4;
        aux = canal_ab[i]&0x0f;
        /*printf("AUX------- %X\n",aux);*/
        canalb[i] = canalb[i] | aux;
    }

    free(canal_a);
    free(canal_b);
    free(canal_ab);

    ad_to_file(canala, canalb, kpc); 
    free(canala);  
    free(canalb);

    return result;
}



/******************************************************************************/


/*  Reset para poder adquiriro leer el buffer*/
bool ad_reset_modo_pc (unsigned int kpc, unsigned int adqm){
    unsigned int conf = 0x00;
    bool result = false;

    result = direccion(0x0b);
    if(result)
        result = escritura(0x80);
    if(result)
        result = escritura(0x00);

    /* modo continuo -> b1 = 1. Modo modulado -> b1 = 0 */
    if(adqm == AD_MODO_CONTINUO)
        conf = conf | 0x02;        
        
    /* Kpc = tamano del buffer de adquisicion en Kb: */    
    if(kpc==64){
            conf=conf|0x70;
    }else if(kpc==32){
            conf=conf|0x60;
    }else if(kpc==16){
            conf=conf|0x50;
    }else if(kpc==8){
            conf=conf|0x40;
    }else if(kpc==4){
            conf=conf|0x30;
    }else if(kpc==2){
            conf=conf|0x20;
    }else if(kpc==1){
            conf=conf|0x10;
    }else if(kpc != 0){
        result = 1;
    }

    if(!result)
        result = direccion(0x0b);   
    if(!result){
        result = escritura(conf|0x80); //Reset contador y modo AD bit b7 = 1  
        result = escritura(conf); //Reset contador y modo AD bit b7 = 0  
    }
    return result;
}



/******************************************************************************/


void ad_to_file(short *canala, short *canalb, unsigned int kpc){

    FILE *output_ad = NULL;
    int i = 0;

    output_ad = fopen("output_ad.txt", "w");
    assert(canala != NULL && canalb != NULL);
    for(i = 0; i < kpc*1024; i++){
          fprintf(output_ad, "canala %u\tcanalb %u\n" , canala[i], canalb[i]);
    }

    fprintf(output_ad, "\n\t\t<< Fin de la muestra >>\n");
    
    fclose(output_ad);
}
