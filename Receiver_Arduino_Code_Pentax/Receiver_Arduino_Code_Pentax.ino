/*----------------------------------------------------------------------------------
     Motion Activated DSLR Shutter Remote
     Receiver Sketch (433mHz Serial RX)
     Written By: Angelo Casimiro (ASCAS/ TechBuilder) 
     Date Written: October 18, 2014
  
     Pin Configuration:
     - Digital#12 of Arduino to Data Out of RX module  
     - Digital#11 of Arduino to IR LED
----------------------------------------------------------------------------------*/
#include <multiCameraIrControl.h> 
#include <VirtualWire.h>

Pentax K7(11); // I Use Nikon (please use the code that works with your DSLR)

void setup()
{
    vw_set_ptt_inverted(true);vw_set_rx_pin(12);
    vw_setup(4000);vw_rx_start();unsigned int data = 0;   
    pinMode(13, OUTPUT);
    pinMode(11, OUTPUT);          
}
void loop(){
  
    uint8_t buf[VW_MAX_MESSAGE_LEN];uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)){
      if(buf[0]=='1'){
        digitalWrite(13,1);
         K7.shutterNow();
      }    
      if(buf[0]=='0'){
       digitalWrite(13,0);
      }
    }
}
