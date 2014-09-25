OPC opc;
PImage dot;

int separacion =10;
int separacionV =3;
int OPC_counter=0;
int initX=50;
int initY=50; //esquina superior izquierda

  // Map one 64-LED strip to the center of the window
//PANELES 1 2 3 4 
int NUM_TIRAS=10*2+3*2; // consideramos las tiras individuales como caso especial de la tira doble
int endX= (NUM_TIRAS-1)*separacion+initX;

int lengthTira=separacionV*24;
int lengthPared=endX-initX;
int endY=lengthTira+initY;

import oscP5.*;
import netP5.*;

void setup()
{
  size(500, 300);
  // Load a sample image
  dot = loadImage("color-dot.png");
  frameRate(25);
  
  // Connect to the local instance of fcserver
  opc = new OPC(this, "127.0.0.1", 7890);
  
  setupOSC();  

int i=0;

// void ledStrip(int index, int count, float x, float y, float spacing, float angle, boolean reversed)
  for(i=0; i<NUM_TIRAS; i++){
    opc.ledStrip(OPC_counter, 24, initX+separacion*i, initY+lengthTira/2, separacionV, -PI/2, false);
    OPC_counter+=24;
    i++;
    opc.ledStrip(OPC_counter, 24, initX+separacion*i, initY+lengthTira/2, separacionV, -PI/2, true);
    OPC_counter+=24;    
  } 
  println(i);
   // opc.ledStrip(0, 24, 100, 100, 2, PI/2, false);
   // opc.ledStrip(24, 24, 120, 100, 2, PI/2, true);
 
  
}

void draw()
{
  
 /* stroke(255,0,0);
  fill(0,0,200);
  line(endX,initY,endX,endY);
  rect(initY,initX,2,2);*/
  
//  rotateDraw(2.0);
//  noiseDraw();
//randomParticleDraw();
waveDraw();
}

