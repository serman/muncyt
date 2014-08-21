/*
 Fade
 
 This example shows how to fade an LED on pin 9
 using the analogWrite() function.
 
 This example code is in the public domain.
 */

int MAXBRILLO = 200;
int led = 6;           // the pin that the LED is attached to
int brightness = 50;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

int brilloTgt;

int lastT;
int difT = 500;
boolean swFading;

// the setup routine runs once when you press reset:
void setup()  { 
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
  
  // start serial port at 9600 bps:
  Serial.begin(9600);
  delay(200);
  Serial.write("\nhola\n");
  delay(20);
  
  analogWrite(led, brightness);    
  lastT = millis();
  delay(20);
  
  swFading = false;
  
  brilloTgt = brightness;
} 

// the loop routine runs over and over again forever:
void loop()  { 
  // set the brightness of pin 9:
  analogWrite(led, brightness);    
  delay(20);

  int tAct = millis();
  
  if( esta en intervalo
      o lo comienza
     ) {
  
    brightness+=fadeAmount;

    // reverse the direction of the fading at the ends of the fade: 
    if (brightness <= 0) {
       brightness = 0;
       fadeAmount = abs(fadeAmount);
    }
    else if(brightness >= MAXBRILLO) {
       brightness = MAXBRILLO;
       fadeAmount = -abs(fadeAmount);
    }
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);
  
  
    if (Serial.available()) {
  //    brightness = Serial.read();
      MAXBRILLO = Serial.read();
      if(brightness>MAXBRILLO) brightness=MAXBRILLO;
  
      
    }

    // set the brightness of the LED:
    analogWrite(led, brightness);
//    // delay 10ms
    delay(20);
//    // read second analog input, divide by 4 to make the range 0-255:
//    secondSensor = analogRead(1)/4;
//    // read  switch, map it to 0 or 255L
//    thirdSensor = map(digitalRead(2), 0, 1, 0, 255);

//    // send sensor values:
    Serial.print(brightness);
    delay(20);
//    Serial.write("\n");
//    delay(20);
//    Serial.write(MAXBRILLO);
//    delay(20);
//    Serial.write("\n");
////    Serial.print(brightness);
//    delay(20);
  }
 

}

