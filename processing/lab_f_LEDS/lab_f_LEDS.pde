OPC opc;
PImage dot;

int separacion =10;
int separacionV =3;
int OPC_counter=0;
int INTERTIRA=50;
int initX=50;
int initY=50; //esquina superior izquierda

  // Map one 64-LED strip to the center of the window
//PANELES 1 2 3 4 
int NUM_TIRAS=10*2+3*2; // consideramos las tiras individuales como caso especial de la tira doble
int endX= (NUM_TIRAS-1)*separacion+initX;
int lengthTramo=separacionV*8;
int lengthTira=separacionV*24+INTERTIRA;
int lengthPared=endX-initX;
int endY=lengthTira+initY;

import oscP5.*;
import netP5.*;
import processing.video.*;

Movie movie;
void setup()
{
  size(700, 700);
  // Load a sample image
  dot = loadImage("color-dot.png");
  frameRate(50);
  
  // Connect to the local instance of fcserver
  opc = new OPC(this, "127.0.0.1", 7890);
  
  setupOSC();  

int i=0;

// void ledStrip(int index, int count, float x, float y, float spacing, float angle, boolean reversed)
  for(i=0; i<NUM_TIRAS; i++){
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2 +INTERTIRA*2, separacionV, -PI/2, false);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2 +INTERTIRA , separacionV, -PI/2, false);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2 , separacionV, -PI/2, false);
    OPC_counter+=8;
    
    
    
    i++;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2, separacionV, -PI/2, true);
    OPC_counter+=8;
     opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2+INTERTIRA, separacionV, -PI/2, true);
    OPC_counter+=8;
    opc.ledStrip(OPC_counter, 8, initX+separacion*i, initY+lengthTramo/2+INTERTIRA*2, separacionV, -PI/2, true);
    OPC_counter+=8;  
   
    
  } 
  println(i);
   // opc.ledStrip(0, 24, 100, 100, 2, PI/2, false);
   // opc.ledStrip(24, 24, 120, 100, 2, PI/2, true);
 
  setupMovie();
  
    //class setup
   // smooth();
  myBall = new Ball[ballAmount];
  for (int ii = 0; ii < ballAmount; ii++) {
    myBall[ii] = new Ball();
    myBall[ii].setup();
  }
  /****/
}

void draw()
{
  
 /* stroke(255,0,0);
  fill(0,0,200);
  line(endX,initY,endX,endY);
  rect(initY,initX,2,2);*/
  
//  rotateDraw(1);
//noiseDraw();
//randomParticleDraw();
waveDraw();
  //breath();
  //drawMovie();
 // drawWeird1();
 //drawTest();
//off();
}


void movieEvent(Movie m) {
  m.read();
}


