#include <stdio.h>

int main (void){
    
 unsigned char byte = 0;
 
 while(1){
 
 
          scanf("%u",&byte);
          _outp(0x378,byte);
          printf("Puerto: %X\n", _inp(0x378));   
 
          if(byte == 'a') break;
}
   
 
 return 0;
}
