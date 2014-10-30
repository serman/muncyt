/*
 Fade
 
 This example shows how to fade an LED on pin 9
 using the analogWrite() function.
 
 This example code is in the public domain.
 */

int MAXBRILLO = 180;
int led = 11;           // the pin that the LED is attached to
int brightness = 230;   // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

boolean bOff = true; 

// the setup routine runs once when you press reset:
void setup()  { 
  
  Serial.begin(9600);
  
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);

  bOff = true;  
    
} 

// the loop routine runs over and over again forever:
void loop()  { 
  
  char inByte = 0;
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    
    if(inByte=='i')      bOff = false;  // encender
    else if(inByte=='o') bOff = true;   // apagar
    
    else if(inByte=='q') {
      // subir brillo maximo en 10
      MAXBRILLO+=10;
    }
    else if(inByte=='a') {
      // bajar brillo maximo en 10
      MAXBRILLO-=10;
    }    
    else if(inByte=='w') {
      // subir brillo maximo en 1
      MAXBRILLO+=1;
    }
    else if(inByte=='s') {
      // bajar brillo maximo en 1
      MAXBRILLO-=1;
    }    
    if(MAXBRILLO>255) MAXBRILLO=255;
    if(MAXBRILLO<0) MAXBRILLO=0;
    
  }

  // set the brightness of pin PWM  
  if(bOff) analogWrite(led, 0);
  else     analogWrite(led, MAXBRILLO);
  
//  analogWrite(led, brightness);    
//
//  // change the brightness for next time through the loop:
//  brightness = brightness + fadeAmount;
//
//  // reverse the direction of the fading at the ends of the fade: 
//  if (brightness == 0 || brightness == MAXBRILLO) {
//    fadeAmount = -fadeAmount ; 
//  }     
//  // wait for 30 milliseconds to see the dimming effect    
//  delay(100);                            
}

