#include <Adafruit_NeoPixel.h>

#define PIN 6
#define PACKET_LENGTH 18
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
int currentRcvPixel,     currentRcvPixel_rcv=0;;

int prevRcvPixel=0;
bool newmsg ;

int colorR=0;
int colorG=0;
int colorB=0;
int colorR_rcv=0;
int colorG_rcv=0;
int colorB_rcv=0;

typedef enum fadestates { RISE, DOWN, ENDED, OFF };
fadestates fadestate=OFF;

char mode, mode_rcv='l';

 char protocolToSend[PACKET_LENGTH];
 char rcvPack[PACKET_LENGTH];

int rcvChar=0;
String rcvAll="";
int fadeCurrentValue=0;
void blink(){
   digitalWrite(13,HIGH);
   delay(400);
   digitalWrite(13,LOW);
   delay(100);
}

void setup() {
  Serial.begin(57600);
  // initialize the LED pin as an output:
  strip.begin();
  clearLEDs();
  strip.show(); // Initialize all pixels to 'off'
 // pinMode(13, OUTPUT);  
}


void loop() {
  

/*   if(byteCounter>PACKET_LENGTH){
     while(Serial.read()!='\n'){       
     }
     byteCounter=0;
   } */

  if(newmsg==true){ //esto significa que hay un msg nuevo completo
          //blink();
//          Serial.println(rcvAll);
          parseMsg();
          rcvAll="";      
          newmsg=false;       
   }
   
   
 /****** MODOS DE FUNCIONAMIENTO *******/  

    if(mode=='l'){ //modo encender todos los pixels hasta el numero recibido
        fadestate=OFF;
     for(int i=0; i<currentRcvPixel; i++) {
          strip.setPixelColor(i, (strip.Color(colorR, colorG, colorB)));        
      }    
      for(int i=currentRcvPixel; i<120; i++) {
          strip.setPixelColor(i,0);        
      }     
      strip.show();
    }  
    else if(mode=='s'){ //modo encender pixel unico
        fadestate=OFF;
      //if(prevRcvPixel !=  currentRcvPixel)
      //strip.setPixelColor(prevRcvPixel,0);
      clearLEDs();
      strip.setPixelColor(currentRcvPixel, strip.Color(colorR, colorG, colorB) ); 
      strip.setPixelColor(currentRcvPixel+1, strip.Color(colorR, colorG, colorB) ); 
      strip.setPixelColor(currentRcvPixel+2, strip.Color(colorR, colorG, colorB) ); 
      strip.setPixelColor(currentRcvPixel+3, strip.Color(colorR, colorG, colorB) ); 
      strip.show();
    }
    
    else if (mode=='o'){      //off
    fadestate=OFF;
      for(int i=0; i<LED_COUNT; i++) {
          strip.setPixelColor(i, (strip.Color(0, 0, 0)));        
      }   
            
          strip.show();
   }
   else if(mode=='f'){//fade To green color fast;
     if (fadestate==OFF){
       fadestate=RISE;
     }
     
     if (fadestate!=ENDED){        
       if(fadestate==RISE){
          fadeCurrentValue+=30;
          if(fadeCurrentValue>=255)     
            fadestate=DOWN;
       }
     if(fadestate==DOWN){
      fadeCurrentValue-=5;
       if(fadeCurrentValue<=0){ fadeCurrentValue=0; fadestate=ENDED;} 
     }
     for (int i=0; i<LED_COUNT; i++)
        {
          strip.setPixelColor(i, (strip.Color(0, fadeCurrentValue, 0)));                 
        }
        strip.show();
   }
  }//end fade mode

   
  establishContact();
  
 
 }
 
void establishContact() {
   Serial.print("0");   // send an initial string
   delay(25);         //within this time we should receive the full msg     
   
}


void parseMsg(){
    mode=(char)rcvAll[0];    
    String num="";
    num+=(char)rcvAll[1];
    num+=(char)rcvAll[2];
    num+=(char)rcvAll[3];
    currentRcvPixel=num.toInt();
    num="";

     //currentRcvPixel=atoi( rcvPack+1); 
    for(int i=4; i<7; i++) num+=(char)rcvAll[i];
    colorR=num.toInt(); num="";
    
    for(int i=7; i<10; i++) num+=(char)rcvAll[i];
    colorG=num.toInt(); num="";

    for(int i=10; i<13; i++) num+=(char)rcvAll[i];
    colorB=num.toInt(); num="";

   // colorR=atoi( &rcvPack[5]); 
   // colorG=atoi( &rcvPack[9]); 
   // colorB=atoi( &rcvPack[13]); 
  /*  Serial.print(colorR);   // send an initial string
   Serial.print(currentRcvPixel);   // send an initial string
   Serial.println(mode);**/

}
 

void serialEvent() {  
     while (Serial.available() > 0) {
     rcvChar = Serial.read();
     if (rcvChar == '\n') {  
        //memcpy(&rcvPack[0], &protocolToSend[0], PACKET_LENGTH);        
        newmsg=true;
        byteCounter=0;  
        //rcvAll="";      
        break;
      }else {
         rcvAll+=(char)rcvChar;
      }
   }
   Serial.flush();
}




void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    strip.setPixelColor(i, 0);
  }
}
