OPC opc;
PImage dot;

int separacion =20;
int OPC_counter=0;
int initX=300;
int initY=400;
  // Map one 64-LED strip to the center of the window
//PANELES 1 2 3 4 
int NUM_TIRAS=10*2+3*2; // consideramos las tiras individuales como caso especial de la tira doble
import oscP5.*;
import netP5.*;

void setup()
{
  size(1024, 768);
  // Load a sample image
  dot = loadImage("color-dot.png");

  // Connect to the local instance of fcserver
  opc = new OPC(this, "127.0.0.1", 7890);
  
  setupOSC();  

int i=0;

// void ledStrip(int index, int count, float x, float y, float spacing, float angle, boolean reversed)
  for(i=0; i<NUM_TIRAS; i++){
    opc.ledStrip(OPC_counter, 24, initX+separacion*i, initY, 2, -PI/2, false);
    OPC_counter+=24;
    i++;
    opc.ledStrip(OPC_counter, 24, initX+separacion*i, initY, 2, -PI/2, true);
    OPC_counter+=24;    
  } 
  println(i);
   // opc.ledStrip(0, 24, 100, 100, 2, PI/2, false);
   // opc.ledStrip(24, 24, 120, 100, 2, PI/2, true);
 
  
}

void draw()
{
  background(0);

  // Draw the image, centered at the mouse location
  float dotSize = width * 0.2;
  image(dot, mouseX - dotSize/2, mouseY - dotSize/2, dotSize, dotSize);
  //fill(100,0,0);
  //rect(mouseX,mouseY,30,30);
}

