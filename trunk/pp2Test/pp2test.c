#include "pp2.h"
#include <stdio.h>
#include <stdlib.h>


int main (void){

    int i = 0;
    unsigned char result[7];
    unsigned char nl = 0x02, ci = 0x01, del[4] = {0,0,0,10}, dat[2] = {3,15}, ps[2] = {0,4};

    printf("Resultado de la funcion: %u\n",build_instruction (result, ps, dat, nl, del, ci));
    
    for(i = 0; i <8; i++){
        printf(">> %X\n", result[i]);
    }
}

