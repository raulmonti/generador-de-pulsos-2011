#include "ad.h"
#include "lpt_windows.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


struct t_ad{
    unsigned char conf; //configuracion actual del ad
    unsigned int modo;  //modulado o continuo
    unsigned int kpc;   //Kbytes de adquisicion por ciclo
    unsigned short *canala;
    unsigned short *canalb;
};


/* FUNCIONES INTERNAS*/
                 
void ad_resetear_contador(ad_t ad);

void ad_set_modo_pc(ad_t ad);

void ad_set_modo_ad(ad_t ad);

bool ad_leer_buffers(ad_t ad);
 

/******************************************************************************/

ad_t ad_create(void){
    
    ad_t ad = NULL;

    ad = calloc(1,sizeof(struct t_ad));
    assert(ad!=NULL);
    ad->conf = 0x00;
    ad->modo = AD_MODO_MODULADO;
    return ad;
}


ad_t ad_destroy(ad_t ad){
    return NULL;
}


bool ad_config (ad_t ad, unsigned int mps, unsigned int kpc, unsigned int adqm){

    bool result = false;  
    unsigned char N = 0;

    /* Modo PC para configurarlo */
    ad_set_modo_pc(ad);
    
    /* Configuracion de la cantidad de muestras por segundo. */

    if(mps < 10000000 && mps > 39215){
        N = 10000000/mps;

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
    if(adqm == AD_MODO_CONTINUO){
        ad->conf = ad->conf | 0x02;
        ad->modo = AD_MODO_CONTINUO;
    }        
        
    /* Kpc = tamano del buffer de adquisicion en Kb: */    
    if(kpc==64){
            ad->conf = ad->conf|0x70;
    }else if(kpc==32){
            ad->conf = ad->conf|0x60;
    }else if(kpc==16){
            ad->conf = ad->conf|0x50;
    }else if(kpc==8){
            ad->conf = ad->conf|0x40;
    }else if(kpc==4){
            ad->conf = ad->conf|0x30;
    }else if(kpc==2){
            ad->conf = ad->conf|0x20;
    }else if(kpc==1){
            ad->conf = ad->conf|0x10;
    }else if(kpc != 0){
        result = 1;
    }
    
    ad->canala = calloc(1, sizeof(short)*kpc*1024);
    ad->canalb = calloc(1, sizeof(short)*kpc*1024);    
    ad->kpc = kpc;

    if(!result)
        result = direccion(0x0b);   
    if(!result){
        result = escritura(ad->conf);
    } 
    return result;
}

/******************************************************************************/


void ad_resetear_contador(ad_t ad){

    assert(ad != NULL);
    direccion(0x0b);
    escritura(ad->conf | 0x80);
    ad->conf = ad->conf&0x7F;
    escritura(ad->conf);

}

/******************************************************************************/

void ad_set_modo_pc(ad_t ad){

    assert(ad!= NULL);

    ad->conf = ad->conf & 0xFE;
    direccion(0x0b);
    escritura(ad->conf);

}

/******************************************************************************/

void ad_set_modo_ad(ad_t ad){

    assert(ad!= NULL);

    ad->conf = ad->conf | 0x01;
    direccion(0x0b);
    escritura(ad->conf);

}

/******************************************************************************/

unsigned int ad_adquirir(ad_t ad){
    unsigned int result = 0;
    unsigned int b = 0;

    //printf("ENTRA\n");

    //RERET Y MODO AD:
    ad_resetear_contador(ad);
    ad_set_modo_ad(ad);
    
    if(!result)          
        result = direccion(0x0b);
    if (!result)
        result = startLeer();    
    if (!result){
        do{
            //printf("ENTRA\n");
            b=leer();
        }while(!(b&0x01));
        result = endLeer();
        printf("leyo b = %u\n",b);
     }
              
    ad_leer_buffers(ad);
                      
    return result;
}



/******************************************************************************/


bool ad_leer_buffers(ad_t ad){
    
    unsigned int i = 0, result = 0;
    char *canal_a = NULL, *canal_b = NULL, *canal_ab = NULL,  aux = 0;
    
    canal_a = calloc(1, sizeof(char)*ad->kpc*1024);
    canal_b = calloc(1, sizeof(char)*ad->kpc*1024);
    canal_ab = calloc(1, sizeof(char)*ad->kpc*1024);    
  
    ad_resetear_contador(ad);
    ad_set_modo_pc(ad);

    direccion(0x08);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(ad->kpc*1024); i++)
        canal_b[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
    
    ad_resetear_contador(ad);
    ad_set_modo_pc(ad);


    direccion(0x09);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(ad->kpc*1024); i++)
        canal_ab[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
        
    ad_resetear_contador(ad);
    ad_set_modo_pc(ad);
    
    direccion(0x0a);                            /* direccion registro de datos */
    startLeer();                                /* bus de datos entrada */
    for(i=0; i<(ad->kpc*1024); i++)
        canal_a[i]=leer();                       /* leeyendo los datos */
    endLeer();                                  /* bus de datos salida */
   
    ad_resetear_contador(ad);

    /* Construyo el resultado final A ab B -> Aa bB*/

    for(i = 0; i<(ad->kpc*1024);i++){
        ad->canala[i] = (canal_ab[i]/16)&0x0f;
        ad->canala[i] += canal_a[i]*16;
    }
    

    for(i = 0; i<(ad->kpc*1024);i++){
        ad->canalb[i] = canal_b[i];
        ad->canalb[i] = ad->canalb[i]<<4;
        aux = canal_ab[i]&0x0f;
        ad->canalb[i] = ad->canalb[i] | aux;
    }

    free(canal_a);
    free(canal_b);
    free(canal_ab);

    return result;
}




/******************************************************************************/


void ad_to_file(ad_t ad){

    FILE *output_ad = NULL;
    int i = 0;

    output_ad = fopen("output_ad.txt", "w");
    assert(ad->canala != NULL && ad->canalb != NULL);
    for(i = 0; i < ad->kpc*1024; i++){
          fprintf(output_ad, "canala %i\tcanalb %i\n" , ad->canala[i], ad->canalb[i]);
    }

    fprintf(output_ad, "\n\t\t<< Fin de la muestra >>\n");
    
    fclose(output_ad);
}
