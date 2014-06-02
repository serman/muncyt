#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
int LED_COUNT=120;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);


int byteCounter=0;
word currentRcvPixel=0;;
word prevRcvPixel=0;
bool newmsg ;
char colorR=0;
char colorG=0;
char colorB=0;
char mode='l';
unsigned char protocolToSend[7];

void blink(){
   digitalWrite(13,HIGH);
   delay(400);
   digitalWrite(13,LOW);
   delay(100);
}

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  strip.begin();
  clearLEDs();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(13, OUTPUT);  
}


void loop() {

  if(newmsg=true){
          //blink();
    mode=protocolToSend[0]; 
    if(prevRcvPixel!=word(protocolToSend[2],protocolToSend[1]))
      prevRcvPixel=currentRcvPixel;
    currentRcvPixel=word(protocolToSend[2],protocolToSend[1]);
    colorR=protocolToSend[3];
    colorG=protocolToSend[4];
    colorB=protocolToSend[5];
    newmsg=false;
  }
  
  if(mode=='l'){ //modo encender todos los pixels hasta el numero recibido
   for(int i=0; i<currentRcvPixel; i++) {
        strip.setPixelColor(i, (strip.Color(colorR, colorG, colorB)));        
    }    
    for(int i=currentRcvPixel; i<LED_COUNT; i++) {
        strip.setPixelColor(i,0);        
    }     
    strip.show();
  }  
  else if(mode=='s'){ //modo encender pixel unico
    if(prevRcvPixel !=  currentRcvPixel)
      strip.setPixelColor(prevRcvPixel,0);
    strip.setPixelColor(currentRcvPixel, (strip.Color(colorR, colorG, colorB))); 
    strip.show();
  }
  
  
  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//  rainbow(20);
//  rainbowCycle(20);

}

void serialEvent() {
   while (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    protocolToSend[byteCounter] = Serial.read();

    if (protocolToSend[byteCounter] == 255) {
      byteCounter=0;
      newmsg=true;
    } else{
      byteCounter++;
    }
  }
}



void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    strip.setPixelColor(i, 0);
  }
}
