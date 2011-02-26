void load_ram_phase(unsigned int mem_address, unsigned int phase_value){
    char buffer = 0;
/* unsigned int lpt_send_byte(unsigned int port_addr, unsigned int data); */
    
    
    

}

/*
#define LPT1_BASE    LPT1
#define LPT1_DATA    LPT1_BASE + 0    
#define LPT1_STATUS  LPT1_BASE + 1
#define LPT1_CONTROL LPT1_BASE + 2

*/


bool set_address(unsigned char address){
    bool result = true;
    
    result = lpt_send_byte(LPT_CONTROL, 0x04);
    
   formMain->DLPortIO1->Port[CONTROL]=0x04; //biow=1,bior=1,le_a=1,enab=1

   delayN(3);

   formMain->DLPortIO1->Port[DATO]=d;   /* direccion en el bus */

   /*  generar el pulso LE_A  */

   formMain->DLPortIO1->Port[CONTROL]=0x07; //biow=1,bior=1,le_a=0,enab=0

   delayN(1);
   formMain->DLPortIO1->Port[CONTROL]=0x05;  //biow=1,bior=1,le_a=1,enab=0

   formMain->DLPortIO1->Port[CONTROL]=0x04;  //biow=1,bior=1,le_a=1,enab=1

}



void escritura(unsigned char b)   /* escritura de un byte de datos */

{

   formMain->DLPortIO1->Port[CONTROL]=0x04; //biow=1,bior=1,le_a=1,enab=1

   delayN(5);

   formMain->DLPortIO1->Port[DATO]=b;

   /* pulso BIOWR */
   formMain->DLPortIO1->Port[CONTROL]=0x0d; //biow=0,bior=1,le_a=1,enab=0
   delayN(8);
   formMain->DLPortIO1->Port[CONTROL]=0x05; //biow=1,bior=1,le_a=1,enab=0

   formMain->DLPortIO1->Port[CONTROL]=0x04; //biow=1,bior=1,le_a=1,enab=1

   delayN(3);
}



void dds_word(unsigned int w)   /* transmite un word en serie  al dds */

{

  unsigned char i,x;

  for(i=0; i<=15; i++){

        x=1;  /* asume bit = 0 */

	if(w&0x8000) x=3;

	escritura(x); /* salida con fsync = 0 */

        /* pulso de reloj */

	x=x&2;

	escritura(x);

	x=x|1;

	escritura(x);

	w=w<<1;  /* desplazar a la izquierda un bit */

  }

  escritura(7);

}

