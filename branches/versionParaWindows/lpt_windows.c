#include "lpt_windows.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>


#define PORT_BUSY 0x80
#define ENABLE_PORT 1
#define DISABLE_PORT 0



/*Las siguientes 2 funciones (inportb y outportb) requieren tener instalado 
 *el driver inpout32.dll en el sistema, y estan pensadas para funcionar en 
 *winXP ya que el mismo no permite el acceso desde la capa de usuario a las 
 *directivas del puerto paralelo sin la utilizacion de algun driver.
 */

/* Definitions in the build of inpout32.dll are:            */
/*   short _stdcall Inp32(short PortAddress);               */
/*   void _stdcall Out32(short PortAddress, short data);    */


/* prototype (function typedef) for DLL function Inp32: */

     typedef short _stdcall (*inpfuncPtr)(short portaddr);
     typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);


short _stdcall inportb(short portAddress){

     HINSTANCE hLib;
     inpfuncPtr inp32;
     short result = 0;
     
     /* Load the library */
     hLib = LoadLibrary("inpout32.dll");

     if (hLib == NULL) {
          printf("LoadLibrary Failed.\n");
          return -1;
     }

     /* get the address of the function */

     inp32 = (inpfuncPtr) GetProcAddress(hLib, "Inp32");

     if (inp32 == NULL) {
          printf("GetProcAddress for Inp32 Failed.\n");
          return -1;
     }
	 
	result = inp32(portAddress);
	 
	freeLibrary(hLib);
	 
	return result;
 
}



void _stdcall outportb(short portAddress, short data){


	HINSTANCE hLib;
    oupfuncPtr oup32;

    /* Load the library */
    hLib = LoadLibrary("inpout32.dll");

    if (hLib == NULL) {
        printf("LoadLibrary Failed.\n");
        return -1;
    }

    oup32 = (oupfuncPtr) GetProcAddress(hLib, "Out32");

     if (oup32 == NULL) {
          printf("GetProcAddress for Oup32 Failed.\n");
          return -1;
     }

	oup32(portAddress, data);
	
	FreeLibrary(hLib);
	 
	return 0;
}






unsigned int lpt_send_byte(unsigned int port_addr, unsigned int data){
	unsigned int status = inportb(port_addr+1);
	unsigned int result = 0;
	if (status == PORT_BUSY){
		result = 1;
	}else{
		outportb(port_addr, data);
	}
	return result;
}

unsigned int lpt_recive_byte(unsigned int port_addr){
 	unsigned int status = inportb(port_addr+1);
	unsigned int result = 0;
	if (status == PORT_BUSY){
		result = 1;
	}else{
	 	result = inportb(port_addr);
	}
	return result;
}

bool lpt_is_busy(unsigned int port_addr){
  unsigned int status = inportb(port_addr+1);
	
	return (status == PORT_BUSY);
}

/*************************************************************************/

/*  rutinas de Walter para comunicarse con el aparato                    */

/*************************************************************************/

/* Cambiar a CHAR el campo data de 'lpt_send_byte' */
bool direccion(unsigned char address){
    bool result = true;
    
    result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    /* Delay() */
    
    if (result)
        result = lpt_send_byte(LPT_DATA, address) == 0;

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x07) == 0;
        /* Delay() */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x05) == 0;
   
   if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
   
   return result;

}



bool escritura(unsigned char data){

    bool result = true;

    result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    /* Delay() */
    
    if (result)
        result = lpt_send_byte(LPT_DATA, data) == 0;

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x0d) == 0;
        /* Delay() */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x05) == 0;    
    
    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
        /* Delay() */    

    return result;
   
}





bool endLeer(void) {

    bool result = true;

    result = lpt_send_byte(LPT_CONTROL, 0x24) == 0;/* bus lpt = entrada */

    if (result)
        result = lpt_send_byte(LPT_CONTROL, 0x04) == 0;
    
    return result;
}




bool startLeer(void) {

    int result;

    result = lpt_send_byte(LPT_CONTROL, 0x24) == 0; /* bus lpt = entrada */
    
    return result;

}





unsigned char leer(void)   /*** leer un byte en la direccion corriente ***/

{   int dto = 0, result = 0;

    /* la funcion leer() devuelve el byte leido */


    result = lpt_send_byte(LPT_CONTROL, 0x21) == 0; /* bus inter_p = salida + RD */

    if(result)
        dto = lpt_recive_byte(LPT_DATA);

    if(result)
        result = lpt_send_byte(LPT_CONTROL, 0x24) == 0;

    return(dto);

}

void setganancia(unsigned int ga, unsigned int gb)
{
 /* clk = d0,  rst/ = d1,  dq = d2*/

 unsigned int i, x, w;

  ga = ga & 0xff;
  gb = gb & 0xff;
  w = (gb * 256) | ga;


       x = 2; /* RST =1 (habilitar DS1267)*/
       escritura(x);

    /* transmite bit de stack =0*/
        x = 3;
        escritura(x);
        x = 2;
        escritura(x);
    /* transmitir los 16 bits de ganacia*/

   for(i=0; i<=15; i++)
     {
          x= 2;  /* asume bit = 0 */

	if(w&0x8000) x=6;

	escritura(x); /* salida con fsync = 0 */

        /* pulso de reloj */

	x=x|1;

	escritura(x);

	x=x&06;

	escritura(x);

	w=w<<1;  /* desplazar a la izquierda un bit */

     }

  escritura(2);
  escritura(0);
}
