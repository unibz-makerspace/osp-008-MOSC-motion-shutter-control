/*----------------------------------------------------------------------------------
     Motion Activated DSLR Shutter Remote
     Transmitter Sketch (433mHz Serial TX)
     By: Angelo Casimiro (ASCAS/ TechBuilder) 
  
     Pin Configuration:
     - Digital#12 of Arduino to Data Input of TX module
     - Analog#0 of Arduino to Motion Detection Alarm - Speaker Pin
----------------------------------------------------------------------------------*/
#include <VirtualWire.h>
char *controller;

void setup() {
  pinMode(13,OUTPUT);
  vw_set_ptt_inverted(true); 
  vw_set_tx_pin(12);
  vw_setup(4000);
}

void loop(){
  
  if(analogRead(A0) > 500){
  //You can add a for loop here to take multiple shots
  controller="1"  ;
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); 
  digitalWrite(13,1);
  delay(10);
  }

  else{
  controller="0";
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); 
  digitalWrite(13,0);
  }
}
